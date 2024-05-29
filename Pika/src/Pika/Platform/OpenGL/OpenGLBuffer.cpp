#include "pkpch.h"
#include "OpenGLBuffer.h"
#include <glad/glad.h>

namespace Pika
{
	////////////////////////////VertexBuffer/////////////////////////////////
	Pika::OpenGLVertexBuffer::OpenGLVertexBuffer(uint32_t vSize)
	{
		glCreateBuffers(1, &m_RendererID);
		glBindBuffer(GL_ARRAY_BUFFER, m_RendererID);
		glBufferData(GL_ARRAY_BUFFER, vSize, nullptr, GL_DYNAMIC_DRAW);
	}

	Pika::OpenGLVertexBuffer::OpenGLVertexBuffer(float* vVertices, uint32_t vSize)
	{
		glCreateBuffers(1, &m_RendererID);
		glBindBuffer(GL_ARRAY_BUFFER, m_RendererID);
		glBufferData(GL_ARRAY_BUFFER, vSize, vVertices, GL_DYNAMIC_DRAW);
	}

	Pika::OpenGLVertexBuffer::~OpenGLVertexBuffer()
	{
		glDeleteBuffers(1, &m_RendererID);
	}

	void Pika::OpenGLVertexBuffer::bind()
	{
		glBindBuffer(GL_ARRAY_BUFFER, m_RendererID);
	}

	void Pika::OpenGLVertexBuffer::unbind()
	{
		glBindBuffer(GL_ARRAY_BUFFER, 0);
	}

	const BufferLayout& OpenGLVertexBuffer::getLayout() const
	{
		return m_Layout;
	}

	void OpenGLVertexBuffer::setLayout(const BufferLayout& vLayout)
	{
		m_Layout = vLayout;
	}

	////////////////////////////IndexBuffer/////////////////////////////////
	OpenGLIndexBuffer::OpenGLIndexBuffer(uint32_t* vIndices, uint32_t vCount)
	{
		m_Count = vCount;
		glCreateBuffers(1, &m_RendererID);
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_RendererID);
		glBufferData(GL_ELEMENT_ARRAY_BUFFER, m_Count * sizeof(uint32_t), vIndices, GL_DYNAMIC_DRAW);
	}
	OpenGLIndexBuffer::~OpenGLIndexBuffer()
	{
		glDeleteBuffers(1, &m_RendererID);
	}
	void OpenGLIndexBuffer::bind()
	{
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_RendererID);
	}
	void OpenGLIndexBuffer::unbind()
	{
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
	}

}
