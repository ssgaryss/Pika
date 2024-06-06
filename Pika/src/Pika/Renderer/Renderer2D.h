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
		static void Submit(const Shader* vShader, const VertexArray* vData, const glm::mat4 vTransform = glm::mat4(1.0f)); //submit Scene info
	};

}
