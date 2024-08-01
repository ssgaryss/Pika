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
		inline glm::vec3 getForwardDirection() const { return glm::rotate(getOrientation(), glm::vec3{ 0.0f, 0.0f, -1.0f }); } // 满足OpenGL习惯，相机看向-z
		inline glm::quat getOrientation() const { return glm::quat(glm::vec3(-m_Pitch, -m_Yaw, 0.0f)); } // 将世界坐标系转换到相机坐标系的四元数
	private:
		void updateCameraViewMatrix();
		void updateCameraProjectionMatrix();
		inline void updatePosition() { m_Position = m_FocalPoint - getForwardDirection() * m_Distance; }
		inline float getRotationSpeed() const { return 0.8f; }
		inline float getZoomSpeed() const { float Distance = std::max(m_Distance * 0.2f, 0.0f); return std::min(Distance * Distance, 100.0f); } // 随着m_Distance值变化而变化
	private:
		// 外参
		glm::vec3 m_Position = { 0.0f, 0.0f, 0.0f };     // 由m_FocalPoint和m_Distance计算得出,可由外部控制器改变位置
		glm::vec3 m_FocalPoint = { 0.0f, 0.0f, -10.0f }; // Zoom操作只改变m_FocalPoint坐标(这里并不是改变FOV)
		float m_Distance = 10.0f;                        // m_FocalPoint沿ForwardDirection方向距离m_Position的距离
		float m_Pitch = 0.0f, m_Yaw = 0.0f;              // 弧度制(这里我就不要roll了感觉用不到)
		glm::mat4 m_ViewMatrix = glm::mat4(1.0f);

		// 内参
		CameraProjectionMode m_ProjectionMode = CameraProjectionMode::Othographic;
		float m_AspectRatio = 1.0f;
		// Othographic
		float m_OthographicSize = 10.0f; // 视口 Height / 2
		float m_OthographicNear = -1.0f, m_OthographicFar = 1.0f;
		// Perspective
		float m_PerspectiveFOV = 45.0f;  // Zoom这里我没有通过改变FOV实现，而是改变光心位置来实现类似效果 
		float m_PerspectiveNear = 0.1f, m_PerspectiveFar = 100.0f;

	};

}
