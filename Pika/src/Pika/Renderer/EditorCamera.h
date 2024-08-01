#pragma once
#include "Camera.h"
#include <glm/glm.hpp>

namespace Pika {

	class EditorCamera : public Camera
	{
	public:
	private:
		// ���
		glm::vec3 m_Position = { 0.0f, 0.0f, 0.0f };
		glm::vec3 m_FocalPoint = { 0.0f, 0.0f, 0.0f };
		float m_Distance = 0.0f; // 
		float m_Yaw = 0.0f, m_Pitch = 0.0f; // �����ҾͲ�Ҫroll�˸о��ò���


		// �ڲ�
		CameraProjectionMode m_ProjectionMode = CameraProjectionMode::Perspective;
		// Othographic
		float m_OthographicSize = 10.0f; // �ӿ� Height / 2
		float m_OthographicNear = -1.0f, m_OthographicFar = 1.0f;
		// Perspective
		float m_PerspectiveFOV = 45.0f;
		float m_PerspectiveNear = 0.1f, m_PerspectiveFar = 100.0f;

		float m_AspectRatio = 1.0f;
	};

}
