#include "pkpch.h"
#include "OpenGLVertexArray.h"
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
		glCreateVertexArrays(1, &m_RendererID);
	}

	OpenGLVertexArray::~OpenGLVertexArray()
	{
		glDeleteVertexArrays(1, &m_RendererID);
	}

	void OpenGLVertexArray::bind()
	{
		glBindVertexArray(m_RendererID);
	}

	void OpenGLVertexArray::unbind()
	{
		glBindVertexArray(0);
	}

	void OpenGLVertexArray::addVertexBuffer(const std::shared_ptr<VertexBuffer>& vVertexBuffer)
	{
		glBindVertexArray(m_RendererID);
		vVertexBuffer->bind();
		const auto& Layout = vVertexBuffer->getLayout();

		m_VertexBuffers.emplace_back(vVertexBuffer);
	}

	const std::vector<std::shared_ptr<VertexBuffer>>& OpenGLVertexArray::getVertexBuffers() const
	{
		return m_VertexBuffers;
	}

	void OpenGLVertexArray::setIndexBuffer(const std::shared_ptr<IndexBuffer>& vIndexBuffer)
	{
		glBindVertexArray(m_RendererID);
		vIndexBuffer->bind();
		m_IndexBuffer = vIndexBuffer;
	}

	const std::shared_ptr<IndexBuffer>& OpenGLVertexArray::getIndexBuffer()
	{
		return m_IndexBuffer;
	}

}