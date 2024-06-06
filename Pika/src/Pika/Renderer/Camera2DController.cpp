#include "pkpch.h"
#include "Camera2DController.h"
#include "Pika/Core/Input.h"

namespace Pika {



	void Camera2DController::onUpdate(Timestep vTimestep)
	{
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
	}

	void Camera2DController::onWindowResizeEvent(WindowResizeEvent& vEvent)
	{
	}

	void Camera2DController::onMouseScrolledEvent(MouseScrolledEvent& vEvent)
	{
	}

}