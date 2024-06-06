#pragma once

#include "Pika/Renderer/Buffer.h"

namespace Pika
{

	class VertexArray //VertexArray can have multiple VertexBuffer and one IndexBuffer
	{
	public:
		virtual ~VertexArray() = default;
		virtual void bind() const = 0;
		virtual void unbind() const = 0;
		
		virtual void addVertexBuffer(const Ref<VertexBuffer>& vVertexBuffer) = 0;
		virtual const std::vector<Ref<VertexBuffer>>& getVertexBuffers() const = 0;
		virtual void setIndexBuffer(const Ref<IndexBuffer>& vIndexBuffer) = 0;
		virtual const Ref<IndexBuffer>& getIndexBuffer() const = 0;

		static Ref<VertexArray> Create();
	};

}

