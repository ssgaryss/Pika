#include "pkpch.h"
#include "EditorCamera.h"
#include "Pika/Core/Input.h"

namespace Pika {

	EditorCamera::EditorCamera()
	{
		updateCameraViewMatrix();
		updateCameraProjectionMatrix();
	}

	EditorCamera::EditorCamera(float vFOV, float vAspectRatio, float vNearClip, float vFarClip)
	{
		m_PerspectiveFOV = std::clamp(vFOV, 10.0f, 150.0f); // 限制范围
		m_AspectRatio = std::clamp(vAspectRatio, 0.00001f, 100000.0f);
		m_PerspectiveNear = std::clamp(vNearClip, 0.00001f, 100000.0f);
		m_PerspectiveFar = std::clamp(vNearClip, m_PerspectiveNear + 0.00001f, m_PerspectiveNear + 100000.0f);
		m_OthographicSize = (m_PerspectiveNear + m_PerspectiveFar) * std::tan(glm::radians(m_PerspectiveFOV) / 2.0f); // 确保mode转换二者视野平滑过度
		m_OthographicNear = m_PerspectiveNear, m_OthographicFar = m_PerspectiveFar;
		updateCameraViewMatrix();
		updateCameraProjectionMatrix();
	}

	void EditorCamera::onUpdate(Timestep vTimestep)
	{
		// TODO : 这里并没有用Shortcut无法更改按键
		if (Input::isKeyPressed(Key::KeyCode::LeftAlt) || Input::isKeyPressed(Key::KeyCode::RightAlt)) {
			const glm::vec2 MousePosition = Input::getMousePosition();
			glm::vec2 Delta = (MousePosition - m_MousePosition) * 0.03f;
			m_MousePosition = MousePosition;
			if (Input::isMouseButtonPressed(Mouse::MouseCode::ButtonLeft))
				onMouseRotate(Delta);
			if (Input::isMouseButtonPressed(Mouse::MouseCode::ButtonMiddle))
				onMousePan(Delta);
			if (Input::isMouseButtonPressed(Mouse::MouseCode::ButtonRight))
				onMouseZoom(Delta.y);
			updatePosition();       // 注意更新m_Position
		}
		if (Input::isMouseButtonPressed(Mouse::MouseCode::ButtonRight)) {
			const glm::vec2 MousePosition = Input::getMousePosition();
			glm::vec2 Delta;
			Delta.x = std::clamp(MousePosition.x - m_MousePosition.x, -50.0f, 50.0f) * 0.03f;
			Delta.y = std::clamp(MousePosition.y - m_MousePosition.y, -50.0f, 50.0f) * 0.03f;
			m_MousePosition = MousePosition;
			onMouseRotate(Delta);
			onKeyMove(vTimestep);   // 注意更新m_FocalPoint
			updateFocalPoint();
		}
		updateCameraViewMatrix();
	}

	void EditorCamera::onEvent(Event& vEvent)
	{
		EventDispatcher Dispatcher(vEvent);
		Dispatcher.dispatch<MouseScrolledEvent>(std::bind_front(&EditorCamera::onMouseScrollZoom, this));
	}

	void EditorCamera::setViewportSize(float vWidth, float vHeight)
	{
		m_AspectRatio = vWidth / vHeight;
		m_ViewportWidth = vWidth;
		m_ViewportHeight = vHeight;
		updateCameraProjectionMatrix();
	}

	void EditorCamera::setPosition(const glm::vec3& vPosition)
	{
		m_Position = vPosition;
		updateFocalPoint();
	}

	void EditorCamera::setFocalPoint(const glm::vec3& vFocalPoint)
	{
		m_FocalPoint = vFocalPoint;
		updatePosition();
	}

	void EditorCamera::updateCameraViewMatrix()
	{
		m_ViewMatrix = glm::lookAt(m_Position, m_FocalPoint, getUpDirection());
	}

	void EditorCamera::updateCameraProjectionMatrix()
	{
		switch (m_ProjectionMode)
		{
		case CameraProjectionMode::Othographic:
		{
			float vLeft = -m_OthographicSize * m_AspectRatio;
			float vRight = m_OthographicSize * m_AspectRatio;
			float vBottom = -m_OthographicSize;
			float vTop = m_OthographicSize;
			m_ProjectionMatrix = glm::ortho(vLeft, vRight, vBottom, vTop, m_OthographicNear, m_OthographicFar);
			return;
		}
		case CameraProjectionMode::Perspective:
		{
			m_ProjectionMatrix = glm::perspective(glm::radians(m_PerspectiveFOV), m_AspectRatio, m_PerspectiveNear, m_PerspectiveFar);
			return;
		}
		}
		PK_CORE_ERROR("EditorCamera : Unknown camera projection mode!");
	}

	void EditorCamera::onKeyMove(Timestep vTimestep)
	{
		if (Input::isKeyPressed(Key::KeyCode::W))
			m_Position += getForwardDirection() * getMoveSpeed();
		if(Input::isKeyPressed(Key::KeyCode::A))
			m_Position += -getRightDirection() * getMoveSpeed();
		if(Input::isKeyPressed(Key::KeyCode::S))
			m_Position += -getForwardDirection() * getMoveSpeed();
		if(Input::isKeyPressed(Key::KeyCode::D))
			m_Position += getRightDirection() * getMoveSpeed();
		if(Input::isKeyPressed(Key::KeyCode::Q))
			m_Position += -getUpDirection() * getMoveSpeed();
		if(Input::isKeyPressed(Key::KeyCode::E))
			m_Position += getUpDirection() * getMoveSpeed();
	}

	void EditorCamera::onMousePan(const glm::vec2& vDelta)
	{
		auto [XSpeed, YSpeed] = getPanSpeed();
		m_FocalPoint += -getRightDirection() * vDelta.x * XSpeed * m_Distance; // 离得越远移动绝对距离也要越大
		m_FocalPoint += getUpDirection() * vDelta.y * YSpeed * m_Distance;
	}

	void EditorCamera::onMouseRotate(const glm::vec2& vDelta)
	{
		float YawDirection = getUpDirection().y < 0 ? -1.0f : 1.0f;  // 当头朝下的时候Yaw旋转的绝对方向反转了（保持相对方向不变）
		m_Yaw += 5.0f * YawDirection * getRotationSpeed() * vDelta.x;
		m_Pitch += 5.0f * getRotationSpeed() * vDelta.y;
	}

	void EditorCamera::onMouseZoom(float vDelta)
	{
		m_Distance -= vDelta * getZoomSpeed();
		if (m_Distance < 1.0f) {
			m_FocalPoint += getForwardDirection();
			m_Distance = 1.0f;
		}
	}

	bool EditorCamera::onMouseScrollZoom(MouseScrolledEvent& vEvent)
	{
		float YOffset = vEvent.getYOffet();
		float SpeedFactor = std::min(std::abs(m_PerspectiveFOV - 10.0f), std::abs(m_PerspectiveFOV - 150.0f)) / 70.0f; // Normalized !
		float ZoomSpeed = 0.5f * (1.2f - static_cast<float>(std::pow(SpeedFactor, 2))); // 当FOV接近最大值150和最小值10时速度逐渐到接近0
		m_PerspectiveFOV = std::clamp(m_PerspectiveFOV - YOffset * ZoomSpeed, 10.0f, 150.0f);
		updateCameraProjectionMatrix();
		return false;
	}

	std::pair<float, float> EditorCamera::getPanSpeed() const
	{
		float XFactor = std::min(m_ViewportWidth / 1000.0f, 2.4f);
		float YFactor = std::min(m_ViewportHeight / 1000.0f, 2.4f);
		float XSpeed = 0.0366f * (XFactor * XFactor) - 0.1778f * XFactor + 0.3021f;
		float YSpeed = 0.0366f * (YFactor * YFactor) - 0.1778f * YFactor + 0.3021f; // Magic Number !
		return std::make_pair(XSpeed, YSpeed);
	}

	float EditorCamera::getRotationSpeed() const
	{
		return 0.8f;
	}

	float EditorCamera::getMoveSpeed() const
	{
		return 0.5f;
	}

	float EditorCamera::getZoomSpeed() const
	{
		float Distance = std::max(m_Distance * 0.2f, 0.0f);
		return std::min(Distance * Distance, 100.0f);
	}

}