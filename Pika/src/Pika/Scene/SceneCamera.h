#pragma once
#include "Pika/Renderer/Camera.h"

namespace Pika
{

	class SceneCamera : public Camera
	{
	public:
		SceneCamera();
		virtual ~SceneCamera() = default;
		void setOthographic(float vOthographicSize, float vOthographicNear, float vOthographicFar);
		void setPerspective(float vPerspectiveFOV, float vPerspectiveNear, float vPerspectiveFar);
		void setViewportSize(uint32_t vWidth, uint32_t vHeight);
		inline float getAspectRatio() const { return m_AspectRatio; }
		// Othographic
		inline float getOthographicSize() const { return m_OthographicSize; }
		inline void setOthographicSize(float vOthographicSize) { m_OthographicSize = vOthographicSize; updateCameraProjectionMatrix(); }
		inline float getOthographicNear() const { return m_OthographicNear; }
		inline void setOthographicNear(float vOthographicNear) { m_OthographicNear = vOthographicNear; updateCameraProjectionMatrix(); }
		inline float getOthographicFar() const { return m_OthographicFar; }
		inline void setOthographicFar(float vOthographicFar) { m_OthographicFar = vOthographicFar; updateCameraProjectionMatrix(); }
		// Perspective
		inline float getPerspectiveFOV() const { return m_PerspectiveFOV; }
		inline void setPerspectiveFOV(float vPerspectiveFOV) { m_PerspectiveFOV = vPerspectiveFOV; updateCameraProjectionMatrix(); }
		inline float getPerspectiveNear() const { return m_PerspectiveNear; }
		inline void setPerspectiveNear(float vPerspectiveNear) { m_PerspectiveNear = vPerspectiveNear; updateCameraProjectionMatrix(); }
		inline float getPerspectiveFar() const { return m_PerspectiveFar; }
		inline void setPerspectiveFar(float vPerspectiveFar) { m_PerspectiveFar = vPerspectiveFar; updateCameraProjectionMatrix(); }
		// ProjectionMode
		inline CameraProjectionMode getProjectionMode() const { return m_ProjectionMode; }
		inline void setProjectionMode(CameraProjectionMode vProjectionMode) { m_ProjectionMode = vProjectionMode; updateCameraProjectionMatrix(); }
	private:
		void updateCameraProjectionMatrix(); // 更新相机内参矩阵(投影矩阵)
	private:
		CameraProjectionMode m_ProjectionMode = CameraProjectionMode::Othographic;
		// Othographic
		float m_OthographicSize = 10.0f; // 视口 Height / 2
		float m_OthographicNear = -1.0f, m_OthographicFar = 1.0f;
		// Perspective
		float m_PerspectiveFOV = 45.0f;
		float m_PerspectiveNear = 0.1f, m_PerspectiveFar = 100.0f;

		float m_AspectRatio = 1.0f;
	};

}
