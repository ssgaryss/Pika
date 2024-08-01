#include "pkpch.h"
#include "EditorCamera.h"

namespace Pika {

	void EditorCamera::updateCameraViewMatrix()
	{
		updatePosition(); // 注意更新, 所有操作都只改变了m_FocalPoint和m_Distance
		m_ViewMatrix = glm::lookAt(m_Position, m_FocalPoint, getUpDirection());
	}
	
	void EditorCamera::updateCameraProjectionMatrix()
	{
		switch (m_ProjectionMode)
		{
		case CameraProjectionMode::Othographic:
		{
			float vLeft = -m_OthographicSize * m_AspectRatio;
			float vRight = m_OthographicSize * m_AspectRatio;
			float vBottom = -m_OthographicSize;
			float vTop = m_OthographicSize;
			m_ProjectionMatrix = glm::ortho(vLeft, vRight, vBottom, vTop, m_OthographicNear, m_OthographicFar);
			return;
		}
		case CameraProjectionMode::Perspective:
		{
			m_ProjectionMatrix = glm::perspective(m_PerspectiveFOV, m_AspectRatio, m_PerspectiveNear, m_PerspectiveFar);
			return;
		}
		}
		PK_CORE_ERROR("EditorCamera : Unknown camera projection mode!");
	}

}