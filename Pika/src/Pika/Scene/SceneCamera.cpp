#include "pkpch.h"
#include "SceneCamera.h"
#include <glm/gtc/matrix_transform.hpp>

namespace Pika {

	SceneCamera::SceneCamera()
	{
		updateCameraProjectionMatrix();
	}

	void SceneCamera::setOthographic(float vOthographicSize, float vOthographicNear, float vOthographicFar)
	{
		m_OthographicSize = vOthographicSize;
		m_OthographicNear = vOthographicNear;
		m_OthographicFar = vOthographicFar;
		updateCameraProjectionMatrix();
	}

	void SceneCamera::setPerspective(float vPerspectiveFOV, float vPerspectiveNear, float vPerspectiveFar)
	{
		m_PerspectiveFOV = vPerspectiveFOV;
		m_PerspectiveNear = vPerspectiveNear;
		m_PerspectiveFar = vPerspectiveFar;
		updateCameraProjectionMatrix();
	}

	void SceneCamera::setViewportSize(uint32_t vWidth, uint32_t vHeight)
	{
		setAspectRatio(static_cast<float>(vWidth) / static_cast<float>(vHeight));
	}

	void SceneCamera::updateCameraProjectionMatrix()
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
			m_ProjectionMatrix = glm::perspective(glm::radians(m_PerspectiveFOV), m_AspectRatio, m_PerspectiveNear, m_PerspectiveFar);
			return;
		}
		}
		PK_CORE_ERROR("SceneCamera : Unknown camera projection mode!");
	}

}