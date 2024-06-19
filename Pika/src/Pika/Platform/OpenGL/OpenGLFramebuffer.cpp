#include "pkpch.h"
#include "OpenGLFramebuffer.h"
#include <glad/glad.h>

namespace Pika {

	namespace FramebufferUtils {

		static GLenum TextureIsMultisample(bool vIsMultisample) {
			return vIsMultisample ? GL_TEXTURE_2D_MULTISAMPLE : GL_TEXTURE_2D;
		}

		static void CreateTexture(bool vIsMultisample, uint32_t* vTextureID, uint32_t vCount) {
			glCreateTextures(TextureIsMultisample(vIsMultisample), vCount, vTextureID);
		}

		static void BindTexuture(bool vIsMultisample, uint32_t vTextureID) {
			glBindTexture(TextureIsMultisample(vIsMultisample), vTextureID);
		}

		static void AttachColorTexture(uint32_t vID, uint32_t vSamples, GLenum vInternalFormat,
			GLenum vDataFormat, uint32_t vWidth, uint32_t vHeight, uint32_t vIndex)
		{
			bool IsMultisample = vSamples > 1;
			if (IsMultisample) {
				glTexImage2DMultisample(GL_TEXTURE_2D_MULTISAMPLE, vSamples, vInternalFormat, vWidth, vHeight, GL_FALSE);
			}
			else {
				glTexImage2D(GL_TEXTURE_2D, 0, vInternalFormat, vWidth, vHeight, 0, vDataFormat, GL_UNSIGNED_BYTE, nullptr);
				glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
				glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
				glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
				glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
				glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);
			}
			glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0 + vIndex, TextureIsMultisample(IsMultisample), vID, 0);
		}

		static void AttachDepthTexture(uint32_t vID, uint32_t vSamples, GLenum vInternalFormat,
			GLenum vAttachmentType, uint32_t vWidth, uint32_t vHeight)
		{
			bool IsMultisample = vSamples > 1;
			if (IsMultisample) {
				glTexImage2DMultisample(GL_TEXTURE_2D_MULTISAMPLE, vSamples, vInternalFormat, vWidth, vHeight, GL_FALSE);
			}
			else {
				glTexStorage2D(GL_TEXTURE_2D, 1, vInternalFormat, vWidth, vHeight);
				glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
				glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
				glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
				glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
				glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);
			}
			glFramebufferTexture2D(GL_FRAMEBUFFER, vAttachmentType, TextureIsMultisample(IsMultisample), vID, 0);
		}


		static bool IsDepthFormat(FramebufferTextureFormat vFormat) {
			switch (vFormat)
			{
			case Pika::FramebufferTextureFormat::Depth: return true;
			}
			return false;
		}
	}


	OpenGLFramebuffer::OpenGLFramebuffer(const FramebufferSpecification& vFramebufferSpecification)
		: m_Specification{ vFramebufferSpecification }
	{
		PK_PROFILE_FUNCTION();
		for (auto& it : vFramebufferSpecification.m_FramebufferAttachments.m_Attachments) {
			if (FramebufferUtils::IsDepthFormat(it.m_TextureFormat)) {
				m_DepthStencilAttachmentSpecification = it;
			}
			else {
				m_ColorAttachmentsSpecification.emplace_back(it);
			}
		}

		refresh();
	}

	OpenGLFramebuffer::~OpenGLFramebuffer()
	{
		PK_PROFILE_FUNCTION();
		glDeleteFramebuffers(1, &m_RendererID);
		glDeleteTextures(1, &m_DepthStencilAttachment);
		glDeleteTextures(static_cast<GLsizei>(m_ColorAttachments.size()), m_ColorAttachments.data());
	}

	void OpenGLFramebuffer::bind()
	{
		PK_PROFILE_FUNCTION();
		glBindFramebuffer(GL_FRAMEBUFFER, m_RendererID);
	}

	void OpenGLFramebuffer::unbind()
	{
		PK_PROFILE_FUNCTION();
		glBindFramebuffer(GL_FRAMEBUFFER, 0);
	}

	void OpenGLFramebuffer::refresh()
	{
		PK_PROFILE_FUNCTION();
		if (!m_RendererID)
		{
			glDeleteFramebuffers(1, &m_RendererID);
			glDeleteTextures(1, &m_DepthStencilAttachment);
			glDeleteTextures(static_cast<GLsizei>(m_ColorAttachments.size()), m_ColorAttachments.data());
			m_DepthStencilAttachment = 0;
			m_ColorAttachments.clear();
		}

		glCreateFramebuffers(1, &m_RendererID);
		glBindFramebuffer(GL_FRAMEBUFFER, m_RendererID);

		bool IsMultisample = m_Specification.m_Samples > 1;

		if (m_ColorAttachmentsSpecification.size()) {
			m_ColorAttachments.resize(m_ColorAttachmentsSpecification.size());
			FramebufferUtils::CreateTexture(IsMultisample, m_ColorAttachments.data(), static_cast<uint32_t>(m_ColorAttachments.size()));
			for (uint32_t i = 0; i < m_ColorAttachments.size(); ++i) {
				FramebufferUtils::BindTexuture(IsMultisample, m_ColorAttachments[i]);
				switch (m_ColorAttachmentsSpecification[i].m_TextureFormat)
				{
				case Pika::FramebufferTextureFormat::RGB8:
					FramebufferUtils::AttachColorTexture(m_ColorAttachments[i], m_Specification.m_Samples,
						GL_RGBA8, GL_RGBA, m_Specification.m_Width, m_Specification.m_Height, i);
					break;
				case Pika::FramebufferTextureFormat::RED_INTEGER:
					FramebufferUtils::AttachColorTexture(m_ColorAttachments[i], m_Specification.m_Samples,
						GL_R32I, GL_RED_INTEGER, m_Specification.m_Width, m_Specification.m_Height, i);
					break;
				default:
					PK_CORE_WARN("OpenGLFramebuffer : Unknown type of color attachment at index {0}.", i);
					break;
				}
			}
		}

		if (m_DepthStencilAttachmentSpecification.m_TextureFormat != FramebufferTextureFormat::None) {
			FramebufferUtils::CreateTexture(IsMultisample, &m_DepthStencilAttachment, 1);
			FramebufferUtils::BindTexuture(IsMultisample, m_DepthStencilAttachment);
			switch (m_DepthStencilAttachmentSpecification.m_TextureFormat)
			{
			case Pika::FramebufferTextureFormat::DEPTH24STENCIL8:
				FramebufferUtils::AttachDepthTexture(m_DepthStencilAttachment, m_Specification.m_Samples,
					GL_DEPTH24_STENCIL8, GL_DEPTH_STENCIL_ATTACHMENT, m_Specification.m_Width, m_Specification.m_Height);
				break;
			default:
				PK_CORE_WARN("OpenGLFramebuffer : Unknown type of depth attachment.");
				break;
			}
		}

		static GLenum Buffers[32] = {
			GL_COLOR_ATTACHMENT0, GL_COLOR_ATTACHMENT1, GL_COLOR_ATTACHMENT2, GL_COLOR_ATTACHMENT3,
			GL_COLOR_ATTACHMENT4, GL_COLOR_ATTACHMENT5, GL_COLOR_ATTACHMENT6, GL_COLOR_ATTACHMENT7,
			GL_COLOR_ATTACHMENT8, GL_COLOR_ATTACHMENT9, GL_COLOR_ATTACHMENT10, GL_COLOR_ATTACHMENT11,
			GL_COLOR_ATTACHMENT12, GL_COLOR_ATTACHMENT13, GL_COLOR_ATTACHMENT14, GL_COLOR_ATTACHMENT15,
			GL_COLOR_ATTACHMENT16, GL_COLOR_ATTACHMENT17, GL_COLOR_ATTACHMENT18, GL_COLOR_ATTACHMENT19,
			GL_COLOR_ATTACHMENT20, GL_COLOR_ATTACHMENT21, GL_COLOR_ATTACHMENT22, GL_COLOR_ATTACHMENT23,
			GL_COLOR_ATTACHMENT24, GL_COLOR_ATTACHMENT25, GL_COLOR_ATTACHMENT26, GL_COLOR_ATTACHMENT27,
			GL_COLOR_ATTACHMENT28, GL_COLOR_ATTACHMENT29, GL_COLOR_ATTACHMENT30, GL_COLOR_ATTACHMENT31,
		};

		GLint MaxColorAttachments;
		glGetIntegerv(GL_MAX_COLOR_ATTACHMENTS, &MaxColorAttachments);

		if (m_ColorAttachments.size() > 1) {
			if (m_ColorAttachments.size() > MaxColorAttachments) {
				PK_CORE_WARN("OpenGLFramebuffer : Your device can support {0} color attachments, but you have {1}.", 
					MaxColorAttachments, m_ColorAttachments.size());
				glDrawBuffers(MaxColorAttachments, Buffers);
			}
			else
				glDrawBuffers(static_cast<GLsizei>(m_ColorAttachments.size()), Buffers);
		}
		else if (m_ColorAttachments.empty()) {
			glDrawBuffer(GL_NONE);
		}

		if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE)
			PK_CORE_ERROR("OpenGLFramebuffer : Framebuffer is incomplete!");

		glBindFramebuffer(GL_FRAMEBUFFER, 0);
	}

}