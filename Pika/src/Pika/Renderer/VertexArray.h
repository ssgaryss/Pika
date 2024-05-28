#pragma once

#include "Pika/Renderer/Buffer.h"

namespace Pika
{

	class VertexArray //VertexArray can have multiple VertexBuffer and one IndexBuffer
	{
	public:
		virtual ~VertexArray() = default;
		virtual void bind() = 0;
		virtual void unbind() = 0;
		
		virtual void addVertexBuffer(const std::shared_ptr<VertexBuffer>& vVertexBuffer) = 0;
		virtual const std::vector<std::shared_ptr<VertexBuffer>>& getVertexBuffers() const = 0;
		virtual void setIndexBuffer(const std::shared_ptr<IndexBuffer>& vIndexBuffer) = 0;
		virtual const std::shared_ptr<IndexBuffer>& getIndexBuffer() = 0;

		static VertexArray* create();
	};

}

