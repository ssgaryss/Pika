#include "pkpch.h"
#include "Camera2DController.h"
#include "Pika/Core/Input.h"

namespace Pika {



	Camera2DController::Camera2DController(float vAspectRation, bool vAllowRotation)
		: m_AspectRatio{ vAspectRation }, m_AllowRotation{ vAllowRotation },
		m_Camera{ -m_AspectRatio * m_ZoomLevel, m_AspectRatio * m_ZoomLevel, -m_ZoomLevel, m_ZoomLevel }
	{
	}

	void Camera2DController::onUpdate(Timestep vTimestep)
	{
		PK_PROFILE_FUNCTION();

		if (Pika::Input::isKeyPressed(Pika::Key::KeyCode::A)) {
			m_Camera.addPosition(glm::vec2(-1.0f * vTimestep, 0.0f));
		}
		else if (Pika::Input::isKeyPressed(Pika::Key::KeyCode::D)) {
			m_Camera.addPosition(glm::vec2(1.0f * vTimestep, 0.0f));
		}

		if (Pika::Input::isKeyPressed(Pika::Key::KeyCode::W)) {
			m_Camera.addPosition(glm::vec2(0.0f, 1.0f * vTimestep));
		}
		else if (Pika::Input::isKeyPressed(Pika::Key::KeyCode::S)) {
			m_Camera.addPosition(glm::vec2(0.0f, -1.0f * vTimestep));
		}

		if (m_AllowRotation) {
			//TODO : Camera2D rotation
			if (Pika::Input::isKeyPressed(Pika::Key::KeyCode::Q)) {
				m_Camera.addRotationDegrees(10.0f);
			}
			else if (Pika::Input::isKeyPressed(Pika::Key::KeyCode::E)) {
				m_Camera.addRotationDegrees(-10.0f);
			}
		}
	}

	void Camera2DController::onEvent(Event& vEvent)
	{
		EventDispatcher Dispatcher(vEvent);
		Dispatcher.dispatch<MouseScrolledEvent>(std::bind(&Camera2DController::onMouseScrolledEvent, this, std::placeholders::_1));
		Dispatcher.dispatch<WindowResizeEvent>(std::bind(&Camera2DController::onWindowResizeEvent, this, std::placeholders::_1));

	}

	bool Camera2DController::onWindowResizeEvent(WindowResizeEvent& vEvent)
	{
		float Width = static_cast<float>(vEvent.getWidth());
		float Height = static_cast<float>(vEvent.getHeight());
		m_AspectRatio = Width / Height;
		m_Camera.setProjectionMatrix(-m_AspectRatio * m_ZoomLevel, m_AspectRatio * m_ZoomLevel, -m_ZoomLevel, m_ZoomLevel);
		return false;
	}

	bool Camera2DController::onMouseScrolledEvent(MouseScrolledEvent& vEvent)
	{
		float Zoom = vEvent.getYOffet() / 10;
		m_ZoomLevel = m_ZoomLevel - Zoom > 0.1f ? m_ZoomLevel - Zoom : 0.1f;
		m_Camera.setProjectionMatrix(-m_AspectRatio * m_ZoomLevel, m_AspectRatio * m_ZoomLevel, -m_ZoomLevel, m_ZoomLevel);
		return false;
	}

}