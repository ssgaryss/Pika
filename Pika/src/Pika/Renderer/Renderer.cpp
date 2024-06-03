#include "pkpch.h"
#include "Renderer.h"
#include "RenderCommand.h"

namespace Pika {

	Scope<Camera> Renderer::s_Camera{ std::make_unique<Camera>(-1.0f, 1.0f, -1.0f, 1.0f) };

	void Renderer::BeginScene()
	{
		s_Camera->updateCamera();
	}

	void Renderer::Submit(const Shader* vShader, const VertexArray* vData, const glm::mat4 vTransform)
	{
		vShader->bind();
		vData->bind();
		vShader->setMat4("u_ViewProjectionMatrix", s_Camera->getViewProjectionMatrix());
		vShader->setMat4("u_Transform", vTransform);
		RenderCommand::DrawIndexed(vData);
	}

	void Renderer::EndScene()
	{
	}

}