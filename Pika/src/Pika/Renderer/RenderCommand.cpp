    #include "pkpch.h"
#include "RenderCommand.h"
#include "RendererAPI.h"


namespace Pika {

	Scope<RendererAPI> RenderCommand::s_RendererAPI{ RendererAPI::Create() };

	void RenderCommand::Init()
	{
		s_RendererAPI->init();
	}

	void RenderCommand::Clear()
	{
		s_RendererAPI->clear();
	}

	void RenderCommand::SetClearColor()
	{
		s_RendererAPI->setClearColor();
	}

	void RenderCommand::SetClearColor(Color vColor)
	{
		s_RendererAPI->setClearColor(vColor);
	}

	void RenderCommand::DrawIndexed(const VertexArray* vVertexArray)
	{
		s_RendererAPI->drawIndexed(vVertexArray);
	}

}