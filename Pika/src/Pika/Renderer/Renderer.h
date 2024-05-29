#pragma once
#include "VertexArray.h"
#include "Shader.h"

namespace Pika
{

	class Renderer
	{
	public:
		static void BeginScene();
		static void Submit(const Shader* vShader, const VertexArray* vData);
		static void EndScene();

	};

}
