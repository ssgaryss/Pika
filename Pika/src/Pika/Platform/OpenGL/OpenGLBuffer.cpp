#include "pkpch.h"
#include "OpenGLBuffer.h"
#include <glad/glad.h>

namespace Pika
{
	////////////////////////////VertexBuffer/////////////////////////////////
	Pika::OpenGLVertexBuffer::OpenGLVertexBuffer(uint32_t vSize)
	{
		PK_PROFILE_FUNCTION();

		glCreateBuffers(1, &m_RendererID);
		glBindBuffer(GL_ARRAY_BUFFER, m_RendererID);
		glBufferData(GL_ARRAY_BUFFER, vSize, nullptr, GL_DYNAMIC_DRAW);
	}

	Pika::OpenGLVertexBuffer::OpenGLVertexBuffer(const void* vVertices, uint32_t vSize)
	{
		PK_PROFILE_FUNCTION();

		glCreateBuffers(1, &m_RendererID);
		glBindBuffer(GL_ARRAY_BUFFER, m_RendererID);
		glBufferData(GL_ARRAY_BUFFER, vSize, vVertices, GL_DYNAMIC_DRAW);
	}

	Pika::OpenGLVertexBuffer::~OpenGLVertexBuffer()
	{
		PK_PROFILE_FUNCTION();

		glDeleteBuffers(1, &m_RendererID);
	}

	void Pika::OpenGLVertexBuffer::bind() const
	{
		PK_PROFILE_FUNCTION();

		glBindBuffer(GL_ARRAY_BUFFER, m_RendererID);
	}

	void Pika::OpenGLVertexBuffer::unbind() const
	{
		PK_PROFILE_FUNCTION();

		glBindBuffer(GL_ARRAY_BUFFER, 0);
	}

	void OpenGLVertexBuffer::setData(const void* vData, uint32_t vSize)
	{
		glBindBuffer(GL_ARRAY_BUFFER, m_RendererID);
		glBufferSubData(GL_ARRAY_BUFFER, 0, vSize, vData);
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
	OpenGLIndexBuffer::OpenGLIndexBuffer(const uint32_t* vIndices, uint32_t vCount)
	{
		PK_PROFILE_FUNCTION();

		m_Count = vCount;
		glCreateBuffers(1, &m_RendererID);
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_RendererID);
		glBufferData(GL_ELEMENT_ARRAY_BUFFER, m_Count * sizeof(uint32_t), vIndices, GL_DYNAMIC_DRAW);
	}
	OpenGLIndexBuffer::~OpenGLIndexBuffer()
	{
		PK_PROFILE_FUNCTION();
		glDeleteBuffers(1, &m_RendererID);
	}
	void OpenGLIndexBuffer::bind() const
	{
		PK_PROFILE_FUNCTION();
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_RendererID);
	}
	void OpenGLIndexBuffer::unbind() const
	{
		PK_PROFILE_FUNCTION();
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
	}

	uint32_t OpenGLIndexBuffer::getCount() const
	{
		return m_Count;
	}

}
