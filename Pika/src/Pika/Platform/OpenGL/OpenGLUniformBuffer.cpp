#include "pkpch.h"
#include "OpenGLUniformBuffer.h"

#include <glad/glad.h>

namespace Pika {



	OpenGLUniformBuffer::OpenGLUniformBuffer(uint32_t vSize, uint32_t vBinding)
	{
		glCreateBuffers(1, &m_RendererID);
		glNamedBufferData(m_RendererID, vSize, nullptr, GL_DYNAMIC_DRAW);
		glBindBufferBase(GL_UNIFORM_BUFFER, vBinding, m_RendererID);
	}

	OpenGLUniformBuffer::~OpenGLUniformBuffer()
	{
		glDeleteBuffers(1, &m_RendererID);
	}

	void OpenGLUniformBuffer::setData(const void* vData, uint32_t vSize, uint32_t vOffset)
	{
		glNamedBufferSubData(m_RendererID, vOffset, vSize, vData);
	}

}