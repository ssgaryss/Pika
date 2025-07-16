    #include "pkpch.h"
#include "RenderCommand.h"
#include "RendererAPI.h"


namespace Pika {

	Scope<RendererAPI> RenderCommand::s_RendererAPI{ RendererAPI::Create() };

	void RenderCommand::Initialize(uint32_t vFlags)
	{
		s_RendererAPI->initialize(vFlags);
	}

	void RenderCommand::Clear()
	{
		s_RendererAPI->clear();
	}

	void RenderCommand::SetClearColor(const glm::vec4& vColor)
	{
		s_RendererAPI->setClearColor(vColor);
	}

	void RenderCommand::DrawIndexed(const VertexArray* vVertexArray, uint32_t vIndexCount)
	{
		s_RendererAPI->drawIndexed(vVertexArray, vIndexCount);
	}

	void RenderCommand::DrawLines(const VertexArray* vVertexArray, uint32_t vIndexCount)
	{
		s_RendererAPI->drawLines(vVertexArray, vIndexCount);
	}

	void RenderCommand::SetLineThickness(float vThickness)
	{
		s_RendererAPI->setLineThickness(vThickness);
	}

	uint32_t RenderCommand::GetAvailableTextureSlots()
	{
		return s_RendererAPI->getAvailableTextureSlots();
	}

	void RenderCommand::DepthMask(bool vAllow)
	{
		return s_RendererAPI->depthMask(vAllow);
	}

}