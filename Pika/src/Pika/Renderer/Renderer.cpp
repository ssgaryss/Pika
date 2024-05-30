#include "pkpch.h"
#include "Renderer.h"

namespace Pika {



	void Renderer::BeginScene()
	{
	}

	void Renderer::Submit(const Shader* vShader)
	{
		vShader->bind();
	}

	void Renderer::EndScene()
	{
	}

}