    #include "pkpch.h"
#include "RenderCommand.h"
#include "RendererAPI.h"


namespace Pika {

	Scope<RendererAPI> RenderCommand::s_RendererAPI{ RendererAPI::Create() };

	void RenderCommand::Initialize()
	{
		s_RendererAPI->Initialize();
	}

	void RenderCommand::Clear()
	{
		s_RendererAPI->clear();
	}

	void RenderCommand::SetClearColor()
	{
		s_RendererAPI->setClearColor();
	}

	void RenderCommand::SetClearColor(const Color& vColor)
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

	uint32_t RenderCommand::getAvailableTextureSlots()
	{
		return s_RendererAPI->getAvailableTextureSlots();
	}

}