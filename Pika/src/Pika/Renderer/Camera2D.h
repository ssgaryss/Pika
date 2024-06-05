#pragma once

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

namespace Pika
{

	class Camera2D
	{
	public:
		Camera2D() = default;
		Camera2D(float vLeft, float vRight, float vBottom, float vTop);

		inline virtual void updateCamera() {
			m_ViewMatrix = glm::lookAt(m_Position, m_Position + glm::vec3(0.0f, 0.0f, -1.0f), glm::vec3(0.0f, 1.0f, 0.0f));
			m_ViewProjectionMatrix = m_ProjectionMatrix * m_ViewMatrix;
		}
		inline virtual void addPosition(glm::vec3 vPositon) { m_Position += vPositon; }

		virtual const glm::vec3 getPosition() const;
		virtual const glm::mat4 getViewMatrix() const;
		virtual const glm::mat4 getProjectionMatrix() const;
		virtual const glm::mat4 getViewProjectionMatrix() const;
	private:
		glm::mat4 m_ProjectionMatrix = glm::mat4(1.0f);
		glm::mat4 m_ViewMatrix = glm::mat4(1.0f);
		glm::mat4 m_ViewProjectionMatrix = glm::mat4(1.0f);
		glm::vec3 m_Position = glm::vec3(0.0f, 0.0f, 1.0f);
	};

}
