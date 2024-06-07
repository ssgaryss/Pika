#pragma once
#include "Shader.h"
#include "Camera2DController.h"
#include "VertexArray.h"

namespace Pika
{

	class Renderer2D
	{
	public:
		Renderer2D() = delete;
		static void Init();
		static void BeginScene(Camera2DController& vCameraController);
		static void EndScene();

		//static
	};

}
