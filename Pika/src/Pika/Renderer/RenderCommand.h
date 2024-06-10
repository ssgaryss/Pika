#pragma once
#include "RendererAPI.h"


namespace Pika
{

	class RenderCommand
	{
	public:
		static void Init();
		static void Clear();
		static void SetClearColor();
		static void SetClearColor(const Color& vColor);
		static void DrawIndexed(const VertexArray* vVertexArray);
	private:
		static Scope<RendererAPI> s_RendererAPI;
	};

}
