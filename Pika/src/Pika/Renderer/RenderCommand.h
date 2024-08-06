#pragma once
#include "RendererAPI.h"


namespace Pika
{

	class RenderCommand
	{
	public:
		static void Initialize();
		static void Clear();
		static void SetClearColor();
		static void SetClearColor(const Color& vColor);
		static void DrawIndexed(const VertexArray* vVertexArray, uint32_t vIndexCount = UINT_MAX);
		static uint32_t getAvailableTextureSlots();
	private:
		static Scope<RendererAPI> s_RendererAPI;
	};

}
