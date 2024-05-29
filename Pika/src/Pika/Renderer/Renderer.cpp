#include "pkpch.h"
#include "Renderer.h"

namespace Pika {



	void Renderer::BeginScene()
	{
	}

	void Renderer::Submit(const Shader* vShader, const VertexArray* vData)
	{
		vShader->bind();
		vData->bind();
	}

	void Renderer::EndScene()
	{
	}

}