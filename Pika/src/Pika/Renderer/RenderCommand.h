#pragma once
#include "RendererAPI.h"


namespace Pika
{

	class RenderCommand
	{
	public:
		static void Initialize(uint32_t vFlags = 0);
		static void Clear();
		static void SetClearColor();
		static void SetClearColor(const Color& vColor);
		static void DrawIndexed(const VertexArray* vVertexArray, uint32_t vIndexCount = UINT_MAX);
		static void DrawLines(const VertexArray* vVertexArray, uint32_t vIndexCount = UINT_MAX);
		static void SetLineThickness(float vThickness);

		static uint32_t GetAvailableTextureSlots();
		static void DepthMask(bool vAllow = true);   // Allow write in depeth buffer or not
	private:
		static Scope<RendererAPI> s_RendererAPI;
	};

}
