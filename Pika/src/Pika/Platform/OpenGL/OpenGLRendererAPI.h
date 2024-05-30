#pragma once
#include "Pika/Renderer/RendererAPI.h"

namespace Pika
{
	class OpenGLRendererAPI : public RendererAPI
	{
	public:

		void clear() override;
		void setClearColor() override;
		void setClearColor(Color vColor) override;
		void drawIndexed(VertexArray* vVertexArray) override;
	};
}

