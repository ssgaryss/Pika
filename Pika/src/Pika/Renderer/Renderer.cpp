#include "pkpch.h"
#include "Renderer.h"
#include "RenderCommand.h"

namespace Pika {

	Scope<Camera> Renderer::s_Camera{ std::make_unique<Camera>(-1.0f, 1.0f, -1.0f, 1.0f) };

	void Renderer::BeginScene()
	{
		s_Camera->updateCamera();
	}

	void Renderer::Submit(const Shader* vShader, const VertexArray* vData)
	{
		vShader->bind();
		vData->bind();
		vShader->setUniformMat4("u_ViewProjectionMatrix", s_Camera->getViewProjectionMatrix());
		RenderCommand::DrawIndexed(vData);
	}

	void Renderer::EndScene()
	{
	}

}