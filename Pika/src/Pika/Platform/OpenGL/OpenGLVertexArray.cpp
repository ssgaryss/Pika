#include "pkpch.h"
#include "OpenGLVertexArray.h"
#include "Pika/Renderer/Buffer.h"
#include <glad/glad.h>

namespace Pika {

	static GLenum convertShaderDataTypeToOpenGLType(ShaderDataType vType) {
		switch (vType)
		{
		case Pika::ShaderDataType::Float:
			return GL_FLOAT;
		case Pika::ShaderDataType::Float2:
			return GL_FLOAT;
		case Pika::ShaderDataType::Float3:
			return GL_FLOAT;
		case Pika::ShaderDataType::Float4:
			return GL_FLOAT;
		case Pika::ShaderDataType::Int:
			return GL_INT;
		case Pika::ShaderDataType::Int2:
			return GL_INT;
		case Pika::ShaderDataType::Int3:
			return GL_INT;
		case Pika::ShaderDataType::Int4:
			return GL_INT;
		case Pika::ShaderDataType::Mat3:
			return GL_FLOAT;
		case Pika::ShaderDataType::Mat4:
			return GL_FLOAT;
		case Pika::ShaderDataType::Bool:
			return GL_BOOL;
		}
		PK_ASSERT(false, "Unknown ShaderDataType!");
		return 0;
	}

	OpenGLVertexArray::OpenGLVertexArray()
	{
		PK_PROFILE_FUNCTION();
		glCreateVertexArrays(1, &m_RendererID);
	}

	OpenGLVertexArray::~OpenGLVertexArray()
	{
		PK_PROFILE_FUNCTION();
		glDeleteVertexArrays(1, &m_RendererID);
	}

	void OpenGLVertexArray::bind() const
	{
		PK_PROFILE_FUNCTION();
		glBindVertexArray(m_RendererID);
	}

	void OpenGLVertexArray::unbind() const
	{
		PK_PROFILE_FUNCTION();
		glBindVertexArray(0);
	}

	void OpenGLVertexArray::addVertexBuffer(const Ref<VertexBuffer>& vVertexBuffer)
	{
		PK_PROFILE_FUNCTION();

		glBindVertexArray(m_RendererID);
		vVertexBuffer->bind();
		const auto& Layout = vVertexBuffer->getLayout();
		for (const auto& it : Layout) {
			glVertexAttribPointer(m_VertexBufferElementIndex,
				it.getComponentCount(), convertShaderDataTypeToOpenGLType(it.m_Type),
				it.m_Normalized ? GL_TRUE : GL_FALSE,
				Layout.getStride(), (void*)it.m_Offset);
			glEnableVertexAttribArray(m_VertexBufferElementIndex);
			m_VertexBufferElementIndex++;
		}
		m_VertexBuffers.emplace_back(vVertexBuffer);
	}

	const std::vector<Ref<VertexBuffer>>& OpenGLVertexArray::getVertexBuffers() const
	{
		return m_VertexBuffers;
	}

	void OpenGLVertexArray::setIndexBuffer(const Ref<IndexBuffer>& vIndexBuffer)
	{
		glBindVertexArray(m_RendererID);
		vIndexBuffer->bind();
		m_IndexBuffer = vIndexBuffer;
	}

	const Ref<IndexBuffer>& OpenGLVertexArray::getIndexBuffer() const
	{
		return m_IndexBuffer;
	}

}