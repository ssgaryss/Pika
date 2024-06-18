#pragma once
#include "Pika/Renderer/VertexArray.h"

namespace Pika
{

	class OpenGLVertexArray : public VertexArray
	{
	public:
		OpenGLVertexArray();
		~OpenGLVertexArray();
		void bind() const override;
		void unbind() const override;

		void addVertexBuffer(const Ref<VertexBuffer>& vVertexBuffer) override;
		void setIndexBuffer(const Ref<IndexBuffer>& vIndexBuffer) override;
		const std::vector<Ref<VertexBuffer>>& getVertexBuffers() const override;
		const Ref<IndexBuffer>& getIndexBuffer() const override;
	private:
		uint32_t m_RendererID = 0;
		std::vector<Ref<VertexBuffer>> m_VertexBuffers;
		Ref<IndexBuffer> m_IndexBuffer;
		uint32_t m_VertexBufferElementIndex = 0; //nums of elements
	};

}

