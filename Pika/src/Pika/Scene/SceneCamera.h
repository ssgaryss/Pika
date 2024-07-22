#pragma once
#include "Pika/Renderer/Camera.h"

namespace Pika
{
	// TODO!
	class SceneCamera : public Camera
	{
	public:

	private:
		CameraProjectionMode m_Mode = CameraProjectionMode::Othographic;
	};

}
