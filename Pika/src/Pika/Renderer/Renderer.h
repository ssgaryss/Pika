#pragma once
#include "VertexArray.h"
#include "Shader.h"

namespace Pika
{

	class Renderer
	{
	public:
		static void BeginScene();
		static void Submit(const Shader* vShader); //submit Scene info
		static void EndScene();

	};

}
