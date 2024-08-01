#pragma once
#include "Camera.h"
#include <glm/glm.hpp>
#include <glm/gtx/quaternion.hpp>

namespace Pika {

	class EditorCamera final : public Camera
	{
	public:
		EditorCamera() = default;


		inline void setViewportSize(float vWidth, float vHeight) { m_AspectRatio = vWidth / vHeight; updateCameraProjectionMatrix(); }
		inline const glm::vec3& getPosition() const { return m_Position; }
		inline float getPitch() const { return m_Pitch; }
		inline float getYaw() const { return m_Yaw; }
		inline const glm::mat4& getViewMatrix() const { return m_ViewMatrix; }
		inline glm::mat4 getViewProjectionMatrix() const { return m_ProjectionMatrix * m_ViewMatrix; }
		inline glm::vec3 getUpDirection() const { return glm::rotate(getOrientation(), glm::vec3{ 0.0f, 1.0f, 0.0f }); }
		inline glm::vec3 getRightDirection() const { return glm::rotate(getOrientation(), glm::vec3{ 1.0f, 0.0f, 0.0f }); }
		inline glm::vec3 getForwardDirection() const { return glm::rotate(getOrientation(), glm::vec3{ 0.0f, 0.0f, -1.0f }); } // ����OpenGLϰ�ߣ��������-z
		inline glm::quat getOrientation() const { return glm::quat(glm::vec3(-m_Pitch, -m_Yaw, 0.0f)); } // ����������ϵת�����������ϵ����Ԫ��
	private:
		void updateCameraViewMatrix();
		void updateCameraProjectionMatrix();
		inline void updatePosition() { m_Position = m_FocalPoint - getForwardDirection() * m_Distance; }
		inline float getRotationSpeed() const { return 0.8f; }
		inline float getZoomSpeed() const { float Distance = std::max(m_Distance * 0.2f, 0.0f); return std::min(Distance * Distance, 100.0f); } // ����m_Distanceֵ�仯���仯
	private:
		// ���
		glm::vec3 m_Position = { 0.0f, 0.0f, 0.0f };     // ��m_FocalPoint��m_Distance����ó�,�����ⲿ�������ı�λ��
		glm::vec3 m_FocalPoint = { 0.0f, 0.0f, -10.0f }; // Zoom����ֻ�ı�m_FocalPoint����(���ﲢ���Ǹı�FOV)
		float m_Distance = 10.0f;                        // m_FocalPoint��ForwardDirection�������m_Position�ľ���
		float m_Pitch = 0.0f, m_Yaw = 0.0f;              // ������(�����ҾͲ�Ҫroll�˸о��ò���)
		glm::mat4 m_ViewMatrix = glm::mat4(1.0f);

		// �ڲ�
		CameraProjectionMode m_ProjectionMode = CameraProjectionMode::Othographic;
		float m_AspectRatio = 1.0f;
		// Othographic
		float m_OthographicSize = 10.0f; // �ӿ� Height / 2
		float m_OthographicNear = -1.0f, m_OthographicFar = 1.0f;
		// Perspective
		float m_PerspectiveFOV = 45.0f;  // Zoom������û��ͨ���ı�FOVʵ�֣����Ǹı����λ����ʵ������Ч�� 
		float m_PerspectiveNear = 0.1f, m_PerspectiveFar = 100.0f;

	};

}
