#include "pkpch.h"
#include "OpenGLUniformBuffer.h"

#include <glad/glad.h>

namespace Pika {



	OpenGLUniformBuffer::OpenGLUniformBuffer(uint32_t vSize, uint32_t vBinding)
	{
		glGenBuffers(1, &m_RendererID);
		glBindBuffer(GL_UNIFORM_BUFFER, m_RendererID);
		glBufferData(GL_UNIFORM_BUFFER, vSize, nullptr, GL_DYNAMIC_DRAW);
		glBindBufferBase(GL_UNIFORM_BUFFER, vBinding, m_RendererID);
	}

	OpenGLUniformBuffer::~OpenGLUniformBuffer()
	{
		glDeleteBuffers(1, &m_RendererID);
	}

	void OpenGLUniformBuffer::setData(const void* vData, uint32_t vSize, uint32_t vOffset)
	{
		glBindBuffer(GL_UNIFORM_BUFFER, m_RendererID);
		glBufferSubData(GL_UNIFORM_BUFFER, vOffset, vSize, vData);
	}

}