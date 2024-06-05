#include "pkpch.h"
#include "Camera2DController.h"
#include "Pika/Core/Input.h"

namespace Pika {



	void Camera2DController::onUpdate(Timestep vTimestep)
	{
		if (Pika::Input::isKeyPressed(Pika::Key::KeyCode::A)) {
			m_Camera.addPosition(glm::vec3(-1.0f * vTimestep, 0.0f, 0.0f));
		}
		else if (Pika::Input::isKeyPressed(Pika::Key::KeyCode::D)) {
			m_Camera.addPosition(glm::vec3(1.0f * vTimestep, 0.0f, 0.0f));
		}

		if (Pika::Input::isKeyPressed(Pika::Key::KeyCode::W)) {
			m_Camera.addPosition(glm::vec3(0.0f, 1.0f * vTimestep, 0.0f));
		}
		else if (Pika::Input::isKeyPressed(Pika::Key::KeyCode::S)) {
			m_Camera.addPosition(glm::vec3(0.0f, -1.0f * vTimestep, 0.0f));
		}

		if (m_AllowRotation) {
			//TODO : Camera2D rotation
			if (Pika::Input::isKeyPressed(Pika::Key::KeyCode::Q)) {
			}
			else if (Pika::Input::isKeyPressed(Pika::Key::KeyCode::E)) {
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