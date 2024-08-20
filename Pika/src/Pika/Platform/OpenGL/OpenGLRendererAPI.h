#pragma once
#include "Pika/Renderer/RendererAPI.h"

namespace Pika
{
	class OpenGLRendererAPI : public RendererAPI
	{
	public:

		void Initialize(uint32_t vFlags) override;
		void clear() override;
		void setClearColor() override;
		void setClearColor(Color vColor) override;
		void drawIndexed(const VertexArray* vVertexArray, uint32_t vIndexCount = UINT_MAX) override;
		void drawLines(const VertexArray* vVertexArray, uint32_t vIndexCount = UINT_MAX) override;
		void setLineThickness(float vThickness) override;
		uint32_t getAvailableTextureSlots() override;
	};
}

