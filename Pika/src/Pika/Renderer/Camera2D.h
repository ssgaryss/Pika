#pragma once

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

namespace Pika
{

	//In 2D mode, camera can only move at the plan where z = 0.0f, Up is +y direction and face to -z direction
	static const float s_Z = 0.0f;
	static const glm::vec3 s_Direction = glm::vec3(0.0f, 0.0f, -1.0f);
	static const glm::vec3 s_Up = glm::vec3(0.0f, 1.0f, 0.0f);

	class Camera2D
	{
	public:
		Camera2D() = default;
		Camera2D(float vLeft, float vRight, float vBottom, float vTop);
		virtual ~Camera2D() = default;


		inline virtual void addPosition(glm::vec2 vPositon) { m_Position += vPositon; updateCameraParameters(); }
		inline virtual void addRotationDegrees(float vRotationDegrees) { m_RotationDegree += vRotationDegrees; updateCameraParameters(); }

		inline virtual const glm::vec2 getPosition() const { return m_Position; }
		inline virtual const glm::mat4 getViewMatrix() const { return m_ViewMatrix; }
		inline virtual const glm::mat4 getProjectionMatrix() const { return m_ProjectionMatrix; }
		inline virtual const glm::mat4 getViewProjectionMatrix() const { return m_ViewProjectionMatrix; }
		inline virtual const void setProjectionMatrix(float vLeft, float vRight, float vBottom, float vTop) {
			m_ProjectionMatrix = glm::ortho(vLeft, vRight, vBottom, vTop, -1.0f, 1.0f);
			m_ViewProjectionMatrix = m_ProjectionMatrix * m_ViewMatrix;
		}
	private:
		virtual void updateCameraParameters();
	private:
		glm::mat4 m_ProjectionMatrix = glm::mat4(1.0f);
		glm::mat4 m_ViewMatrix = glm::mat4(1.0f);
		glm::mat4 m_ViewProjectionMatrix = glm::mat4(1.0f);
		glm::vec2 m_Position = glm::vec2(0.0f, 0.0f);
		float m_RotationDegree = 0.0f; // z axis
	};

}
