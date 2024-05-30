#pragma once
#include "RendererAPI.h"


namespace Pika
{

	class RenderCommand
	{
	public:
		static void Clear();
		static void SetClearColor();
		static void SetClearColor(Color vColor);
		static void DrawIndexed(VertexArray* vVertexArray);
	private:
		static std::unique_ptr<RendererAPI> s_RendererAPI;
	};

}
