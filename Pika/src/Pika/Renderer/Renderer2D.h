#pragma once
#include "Camera2DController.h"
#include <glm/glm.hpp>

namespace Pika
{

	class Renderer2D
	{
	public:
		Renderer2D() = delete;
		static void Init();
		static void BeginScene(Camera2DController& vCameraController);
		static void EndScene();

		static void drawQuad(const glm::vec2& vPosition, const glm::vec2& vScale, const glm::vec4& vColor);

		//static
	};

}
