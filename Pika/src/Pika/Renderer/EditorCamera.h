#pragma once
#include "Camera.h"
#include "Pika/Events/Event.h"
#include "Pika/Events/MouseEvent.h"
#include "Pika/Core/Timestep.h"
#include <glm/glm.hpp>
#include <glm/gtx/quaternion.hpp>

namespace Pika {

	class EditorCamera final : public Camera
	{
	public:
		EditorCamera();
		EditorCamera(float vFOV, float vAspectRatio, float vNearClip, float vFarClip); // ֻ��עPerspective mode,����ģʽ������͸��ģʽ�����Ƶ�

		void onUpdate(Timestep vTimestep);
		void onEvent(Event& vEvent);

		void setViewportSize(float vWidth, float vHeight);
		void setPosition(const glm::vec3& vPosition);
		void setFocalPoint(const glm::vec3& vFocalPoint);

		inline bool isOthograhic() const { return m_ProjectionMode == CameraProjectionMode::Othographic; }
		inline const glm::vec3& getPosition() const { return m_Position; }
		inline float getPitch() const { return m_Pitch; }
		inline float getYaw() const { return m_Yaw; }
		inline const glm::mat4& getViewMatrix() const { return m_ViewMatrix; }
		inline glm::mat4 getViewProjectionMatrix() const { return m_ProjectionMatrix * m_ViewMatrix; }
		inline glm::vec3 getUpDirection() const { return glm::rotate(getOrientation(), glm::vec3{ 0.0f, 1.0f, 0.0f }); }
		inline glm::vec3 getRightDirection() const { return glm::rotate(getOrientation(), glm::vec3{ 1.0f, 0.0f, 0.0f }); }
		inline glm::vec3 getForwardDirection() const { return glm::rotate(getOrientation(), glm::vec3{ 0.0f, 0.0f, -1.0f }); } // ����OpenGLϰ�ߣ��������-z
		inline glm::quat getOrientation() const { return glm::quat(glm::radians(glm::vec3(-m_Pitch, -m_Yaw, 0.0f))); } // ����������ϵת�����������ϵ����Ԫ��
	private:
		void updateCameraViewMatrix();
		void updateCameraProjectionMatrix();
		inline void updatePosition() { m_Position = m_FocalPoint - getForwardDirection() * m_Distance; }
		inline void updateFocalPoint() { m_FocalPoint = m_Position + getForwardDirection() * m_Distance; }

		void onKeyMove(Timestep vTimestep);                       // w,a,s,d�����ƶ� 
		void onMousePan(const glm::vec2& vDelta);                 // �����ƶ�
		void onMouseRotate(const glm::vec2& vDelta);              // ��ת,�ı�m_Yaw,m_Pitch
		void onMouseZoom(float vDelta);                           // �ı�Distance
		bool onMouseScrollZoom(MouseScrolledEvent& vEvent);       // �ı�FOV

		std::pair<float, float> getPanSpeed() const;     // �����ƶ��ٶȣ�x����y����
		float getRotationSpeed() const;                  // Yaw, Pitch ���ٶ�
		float getMoveSpeed() const;                      // Move�ٶ�
		float getZoomSpeed() const;                      // Zoom�ٶ�,����m_Distanceֵ�仯���仯(���λ��Խ�ӽ������ٶ�ԽС)
	private:
		// Mouse���
		glm::vec2 m_MousePosition = { 0.0f, 0.0f };      // ��¼��һ��ʱ�����λ��
		// Viewport���
		float m_ViewportWidth = 1000.0f, m_ViewportHeight = 1000.0f;
		// ���
		glm::vec3 m_Position = { 0.0f, 0.0f, 10.0f };    // ��m_FocalPoint��m_Distance����ó�,�����ⲿ�������ı�λ��
		glm::vec3 m_FocalPoint = { 0.0f, 0.0f, 0.0f };   // ����
		float m_Distance = 10.0f;                        // m_FocalPoint��ForwardDirection�������m_Position�ľ���,��СΪ1.0f
		float m_Pitch = 0.0f, m_Yaw = 0.0f;              // �Ƕ���(�����ҾͲ�Ҫroll�˸о��ò���)
		glm::mat4 m_ViewMatrix = glm::mat4(1.0f);

		// �ڲ�
		CameraProjectionMode m_ProjectionMode = CameraProjectionMode::Perspective;
		float m_AspectRatio = 1.0f;
		// Othographic
		float m_OthographicSize = 10.0f; // �ӿ� Height / 2
		float m_OthographicNear = 0.1f, m_OthographicFar = 100.0f;
		// Perspective
		float m_PerspectiveFOV = 45.0f;  // Zoom��û��ͨ���ı�FOVʵ�֣����Ǹı�m_Distance��Ӹı������λ����ʵ������Ч�� 
		float m_PerspectiveNear = 0.1f, m_PerspectiveFar = 100.0f;

	};

}
