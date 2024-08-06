#include "pkpch.h"
#include "Renderer3D.h"
#include "RenderCommand.h"

namespace Pika {

	Scope<Camera2D> Renderer3D::s_Camera{ std::make_unique<Camera2D>(-1.0f, 1.0f, -1.0f, 1.0f) };

	void Renderer3D::Initialize()
	{
		RenderCommand::Initialize();
	}

	void Renderer3D::BeginScene()
	{
	}

	void Renderer3D::Submit(const Shader* vShader, const VertexArray* vData, const glm::mat4 vTransform)
	{
		vShader->bind();
		vData->bind();
		vShader->setMat4("u_ViewProjectionMatrix", s_Camera->getViewProjectionMatrix());
		vShader->setMat4("u_Transform", vTransform);
		RenderCommand::DrawIndexed(vData);
	}

	void Renderer3D::EndScene()
	{
	}

}