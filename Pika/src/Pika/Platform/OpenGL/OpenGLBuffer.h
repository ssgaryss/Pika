#pragma once
#include "Pika/Renderer/Buffer.h"

namespace Pika
{

	class OpenGLVertexBuffer : public VertexBuffer
	{
	public:
		OpenGLVertexBuffer(uint32_t vSize);
		OpenGLVertexBuffer(float* vVertices, uint32_t vSize);
		OpenGLVertexBuffer(const OpenGLVertexBuffer& vVertexBuffer) = default;
		OpenGLVertexBuffer& operator=(const OpenGLVertexBuffer& rhs) = default;
		~OpenGLVertexBuffer() override;
		void bind() const override;
		void unbind() const override;
		void setData(const void* vData, uint32_t vSize) override;

		const BufferLayout& getLayout() const override;
		void setLayout(const BufferLayout& vLayout) override;
	private:
		uint32_t m_RendererID;
		BufferLayout m_Layout; //describe each part of VBO represent what type of data
	};


	class OpenGLIndexBuffer : public IndexBuffer
	{
	public:
		OpenGLIndexBuffer() = delete;
		OpenGLIndexBuffer(uint32_t* vIndices, uint32_t vCount);
		~OpenGLIndexBuffer();
		void bind() const override;
		void unbind() const override;
		uint32_t getCount() const override;
	private:
		uint32_t m_RendererID;
		uint32_t m_Count; //nums of indices
	};

}

