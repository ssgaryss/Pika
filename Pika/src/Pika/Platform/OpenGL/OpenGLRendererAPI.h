#pragma once
#include "Pika/Renderer/RendererAPI.h"

namespace Pika
{
	class OpenGLRendererAPI : public RendererAPI
	{
	public:

		void init() override;
		void clear() override;
		void setClearColor() override;
		void setClearColor(Color vColor) override;
		void drawIndexed(const VertexArray* vVertexArray) override;
	};
}
