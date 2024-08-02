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
		m_OthographicSize = 2.0f * m_PerspectiveNear * std::tan(glm::radians(m_PerspectiveFOV) / 2.0f) * 50.0f; // 这里50是我取得一个常数,感觉合理即可
		m_OthographicNear = m_PerspectiveNear, m_OthographicFar = m_PerspectiveFar;
		updateCameraViewMatrix();
		updateCameraProjectionMatrix();
	}

	void EditorCamera::onUpdate(Timestep vTimestep)
	{
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
			PK_CORE_ERROR("FocalPoint : x = {}, y = {}, z = {}", m_FocalPoint.x, m_FocalPoint.y, m_FocalPoint.z);
			PK_CORE_ERROR("Distance : {}", m_Distance);
		}
		updateCameraViewMatrix();
	}

	void EditorCamera::onEvent(Event& vEvent)
	{
	}

	void EditorCamera::setViewportSize(float vWidth, float vHeight)
	{
		m_AspectRatio = vWidth / vHeight;
		m_ViewportWidth = vWidth;
		m_ViewportHeight = vHeight;
		updateCameraProjectionMatrix();
	}

	void EditorCamera::updateCameraViewMatrix()
	{
		updatePosition(); // 注意更新, 所有操作都只改变了m_FocalPoint和m_Distance
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
			m_ProjectionMatrix = glm::perspective(m_PerspectiveFOV, m_AspectRatio, m_PerspectiveNear, m_PerspectiveFar);
			return;
		}
		}
		PK_CORE_ERROR("EditorCamera : Unknown camera projection mode!");
	}

	void EditorCamera::onMousePan(const glm::vec2& vDelta)
	{
		// TODO
	}

	void EditorCamera::onMouseRotate(const glm::vec2& vDelta)
	{
		// TODO
	}

	void EditorCamera::onMouseZoom(float vDelta)
	{
		m_Distance -= vDelta * getZoomSpeed();
		if (m_Distance < 1.0f) {
			m_FocalPoint += getForwardDirection();
			m_Distance = 1.0f;
		}
	}

	std::pair<float, float> EditorCamera::getPanSpeed() const
	{
		float XSpeed = std::min(m_ViewportWidth / 1000.0f, 2.4f);
		float YSpeed = std::min(m_ViewportHeight / 1000.0f, 2.4f);
		float XFactor = 0.0366f * (XSpeed * XSpeed) - 0.1778f * XSpeed + 0.3021f;
		float YFactor = 0.0366f * (YSpeed * YSpeed) - 0.1778f * YSpeed + 0.3021f; // Magic Number !
		return std::make_pair(XFactor, YFactor);
	}

	float EditorCamera::getRotationSpeed() const
	{
		return 0.8f;
	}

	float EditorCamera::getZoomSpeed() const
	{
		float Distance = std::max(m_Distance * 0.2f, 0.0f);
		return std::min(Distance * Distance, 100.0f);
	}

}