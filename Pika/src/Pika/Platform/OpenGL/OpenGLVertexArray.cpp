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
		for (const auto& Element : Layout) {
			switch (Element.m_Type)
			{
			case ShaderDataType::Float:
			case ShaderDataType::Float2:
			case ShaderDataType::Float3:
			case ShaderDataType::Float4:
			{
				glEnableVertexAttribArray(m_VertexBufferElementIndex);
				glVertexAttribPointer(m_VertexBufferElementIndex,
					Element.getComponentCount(), convertShaderDataTypeToOpenGLType(Element.m_Type),
					Element.m_Normalized ? GL_TRUE : GL_FALSE,
					Layout.getStride(), (const void*)Element.m_Offset);
				m_VertexBufferElementIndex++;
				break;
			}
			case ShaderDataType::Int: 
			case ShaderDataType::Int2:
			case ShaderDataType::Int3:
			case ShaderDataType::Int4:
			case ShaderDataType::Bool:
			{
				glEnableVertexAttribArray(m_VertexBufferElementIndex);
				glVertexAttribIPointer(m_VertexBufferElementIndex,
					Element.getComponentCount(), convertShaderDataTypeToOpenGLType(Element.m_Type),
					Layout.getStride(), (const void*)Element.m_Offset);
				m_VertexBufferElementIndex++;
				break;
			}
			case ShaderDataType::Mat3:
			case ShaderDataType::Mat4:
			{
				uint8_t Count = Element.getComponentCount(); // Count × Count
				for (uint8_t i = 0; i < Count; ++i) {
					glEnableVertexAttribArray(m_VertexBufferElementIndex);
					glVertexAttribPointer(m_VertexBufferElementIndex,
						Count, convertShaderDataTypeToOpenGLType(Element.m_Type),
						Element.m_Normalized ? GL_TRUE : GL_FALSE,
						Layout.getStride(), (const void*)(Element.m_Offset + sizeof(float) * Count * i));
					glVertexAttribDivisor(m_VertexBufferElementIndex, 1);  // 一般情况下每个VAO共享一个Mat值
					m_VertexBufferElementIndex++;
				}
				break;
			}
			case ShaderDataType::None:
			{
				PK_CORE_ERROR("OpenGLVertexArray : Unknown shader data type!");
				return;
			}
			}
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