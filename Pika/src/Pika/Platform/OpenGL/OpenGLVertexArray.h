#pragma once
#include "Pika/Renderer/VertexArray.h"

namespace Pika
{

	class OpenGLVertexArray : public VertexArray
	{
	public:
		OpenGLVertexArray();
		~OpenGLVertexArray();
		void bind() override;
		void unbind() override;

		void addVertexBuffer(const std::shared_ptr<VertexBuffer>& vVertexBuffer) override;
		void setIndexBuffer(const std::shared_ptr<IndexBuffer>& vIndexBuffer) override;
		const std::vector<std::shared_ptr<VertexBuffer>>& getVertexBuffers() const override;
		const std::shared_ptr<IndexBuffer>& getIndexBuffer() override;
	private:
		uint32_t m_RendererID;
		std::vector<std::shared_ptr<VertexBuffer>> m_VertexBuffers;
		std::shared_ptr<IndexBuffer> m_IndexBuffer;
		uint32_t m_VertexBufferElementIndex = 0;
	};

}

