#pragma once
#include "Pika/Renderer/Buffer.h"

namespace Pika
{

	class OpenGLVertexBuffer : public VertexBuffer
	{
	public:
		OpenGLVertexBuffer(uint32_t vSize);
		OpenGLVertexBuffer(float* vVertices, uint32_t vSize);
		~OpenGLVertexBuffer() override;
		void bind() override;
		void unbind() override;
	private:
		uint32_t m_RendererID;

	};


	class OpenGLIndexBuffer : public IndexBuffer
	{
	public:
		OpenGLIndexBuffer(uint32_t vSize);
		OpenGLIndexBuffer(uint32_t* vIndices, uint32_t vSize);
		~OpenGLIndexBuffer();
		void bind() override;
		void unbind() override;
	private:
		uint32_t m_RendererID;
	};

}

