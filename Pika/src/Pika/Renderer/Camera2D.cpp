#include "pkpch.h"
#include "Camera2D.h"


namespace Pika
{
	Pika::Camera2D::Camera2D(float vLeft, float vRight, float vBottom, float vTop)
	{
		m_ViewMatrix = glm::lookAt(glm::vec3(m_Position, s_Z), glm::vec3(m_Position, s_Z) + s_Direction, s_Up);
		m_ProjectionMatrix = glm::ortho(vLeft, vRight, vBottom, vTop);
		m_ViewProjectionMatrix = m_ProjectionMatrix * m_ViewMatrix;
	}

	void Camera2D::addRotationDegrees(float vRotationDegrees)
	{
		if (m_RotationDegree + vRotationDegrees > 89.0f)
		{
			m_RotationDegree = 89.0f;
		}
		else if (m_RotationDegree + vRotationDegrees < -89.0f) {
			m_RotationDegree = -89.0f;
		}
		else {
			m_RotationDegree += vRotationDegrees;
		}
		updateCameraParameters();

	}

	void Camera2D::updateCameraParameters()
	{
		glm::mat4 Transform = glm::translate(glm::mat4(1.0f), glm::vec3(m_Position, s_Z))
			* glm::rotate(glm::mat4(1.0f), glm::radians(m_RotationDegree), s_Direction);
		m_ViewMatrix = glm::inverse(Transform);
		m_ViewProjectionMatrix = m_ProjectionMatrix * m_ViewMatrix;
	}

}
