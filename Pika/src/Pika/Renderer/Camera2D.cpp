#include "pkpch.h"
#include "Camera2D.h"


namespace Pika
{
	Pika::Camera2D::Camera2D(float vLeft, float vRight, float vBottom, float vTop)
	{
		m_ViewMatrix = glm::lookAt(glm::vec3(m_Position, s_Z), glm::vec3(m_Position, s_Z) + s_Direction, s_Up);
		m_ProjectionMatrix = glm::ortho(vLeft, vRight, vBottom, vTop, -1.0f, 1.0f);
		m_ViewProjectionMatrix = m_ProjectionMatrix * m_ViewMatrix;
	}

	void Camera2D::updateCameraParameters()
	{
		glm::mat4 Transform = glm::translate(glm::mat4(1.0f), glm::vec3(m_Position, s_Z))
			* glm::rotate(glm::mat4(1.0f), glm::radians(m_RotationDegree), s_Direction);
		m_ViewMatrix = glm::inverse(Transform);
		m_ViewProjectionMatrix = m_ProjectionMatrix * m_ViewMatrix;
	}

}
