#include "pkpch.h"
#include "OpenGLFramebuffer.h"
#include "OpenGLTextureFormat.h"
#include <glad/glad.h>

namespace Pika {

	namespace Utils {

		static GLenum TextureIsMultisample(bool vIsMultisample) {
			return vIsMultisample ? GL_TEXTURE_2D_MULTISAMPLE : GL_TEXTURE_2D;
		}

		static void CreateTexture(bool vIsMultisample, uint32_t* vTextureID, uint32_t vCount) {
			PK_PROFILE_FUNCTION();
			glCreateTextures(TextureIsMultisample(vIsMultisample), vCount, vTextureID);
		}

		static void BindTexuture(bool vIsMultisample, uint32_t vTextureID) {
			PK_PROFILE_FUNCTION();
			glBindTexture(TextureIsMultisample(vIsMultisample), vTextureID);
		}

		static void AttachColorTexture(uint32_t vID, uint32_t vSamples, GLenum vInternalFormat,
			uint32_t vWidth, uint32_t vHeight, uint32_t vIndex)
		{
			PK_PROFILE_FUNCTION();
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
			glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0 + vIndex, TextureIsMultisample(IsMultisample), vID, 0);
		}

		static void AttachDepthTexture(uint32_t vID, uint32_t vSamples, GLenum vInternalFormat,
			GLenum vAttachmentType, uint32_t vWidth, uint32_t vHeight)
		{
			PK_PROFILE_FUNCTION();
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


		static bool IsDepthFormat(TextureFormat vFormat) {
			switch (vFormat)
			{
			case Pika::TextureFormat::DEPTH16: return true;
			case Pika::TextureFormat::DEPTH32F: return true;
			case Pika::TextureFormat::DEPTH24STENCIL8: return true;
			}
			return false;
		}
	}


	OpenGLFramebuffer::OpenGLFramebuffer(const FramebufferSpecification& vFramebufferSpecification)
		: m_Specification{ vFramebufferSpecification }
	{
		PK_PROFILE_FUNCTION();
		for (auto& it : vFramebufferSpecification.m_FramebufferAttachments.m_Attachments) {
			if (Utils::IsDepthFormat(it.m_TextureFormat)) {
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
			Utils::CreateTexture(IsMultisample, m_ColorAttachments.data(), static_cast<uint32_t>(m_ColorAttachments.size()));
			for (uint32_t i = 0; i < m_ColorAttachments.size(); ++i) {
				Utils::BindTexuture(IsMultisample, m_ColorAttachments[i]);
				auto InternalFormat = Utils::PikaTextureFormatToGLInternalFormat(m_ColorAttachmentsSpecification[i].m_TextureFormat);

				Utils::AttachColorTexture(m_ColorAttachments[i], m_Specification.m_Samples,
					InternalFormat, m_Specification.m_Width, m_Specification.m_Height, i);
				if (!InternalFormat)
					PK_CORE_WARN("OpenGLFramebuffer : Unknown type of color attachment at index {0}.", i);
			}
		}

		if (m_DepthStencilAttachmentSpecification.m_TextureFormat != TextureFormat::None) {
			Utils::CreateTexture(IsMultisample, &m_DepthStencilAttachment, 1);
			Utils::BindTexuture(IsMultisample, m_DepthStencilAttachment);
			switch (m_DepthStencilAttachmentSpecification.m_TextureFormat)
			{
			case Pika::TextureFormat::DEPTH24STENCIL8:
				Utils::AttachDepthTexture(m_DepthStencilAttachment, m_Specification.m_Samples,
					GL_DEPTH24_STENCIL8, GL_DEPTH_STENCIL_ATTACHMENT, m_Specification.m_Width, m_Specification.m_Height);
				break;
			default:
				PK_CORE_WARN("OpenGLFramebuffer : Unknown type of depth attachment.");
				break;
			}
		}

		static GLenum Buffers[32] = {
			GL_COLOR_ATTACHMENT0,  GL_COLOR_ATTACHMENT1,  GL_COLOR_ATTACHMENT2,  GL_COLOR_ATTACHMENT3,
			GL_COLOR_ATTACHMENT4,  GL_COLOR_ATTACHMENT5,  GL_COLOR_ATTACHMENT6,  GL_COLOR_ATTACHMENT7,
			GL_COLOR_ATTACHMENT8,  GL_COLOR_ATTACHMENT9,  GL_COLOR_ATTACHMENT10, GL_COLOR_ATTACHMENT11,
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

	void OpenGLFramebuffer::setViewport(uint32_t vLBX, uint32_t vLBY, uint32_t vRTX, uint32_t vRTY)
	{
		uint32_t Width = m_Specification.m_Width;
		uint32_t Height = m_Specification.m_Height;
		if (vLBX >= vRTX || vLBY >= vRTY || vLBX >= Width || vLBY >= Height || vRTX >= Width || vRTY >= Height) {
			PK_CORE_ERROR("OpenGLFramebuffer : Invalid viewport parameters!");
			return;
		}
		glViewport(vLBX, vLBY, vRTX, vRTY);
	}

	void OpenGLFramebuffer::resize(uint32_t vWidth, uint32_t vHeight)
	{
		PK_PROFILE_FUNCTION();
		if (vWidth > GL_MAX_FRAMEBUFFER_WIDTH || vHeight > GL_MAX_FRAMEBUFFER_HEIGHT)
			PK_CORE_ERROR("OpenGLFramebuffer : Invalid framebuffer size ({0}, {1}), the maximum size is ({2}, {3})!",
				vWidth, vHeight, GL_MAX_FRAMEBUFFER_WIDTH, GL_MAX_FRAMEBUFFER_HEIGHT);
		m_Specification.m_Width = vWidth;
		m_Specification.m_Height = vHeight;
		refresh();
	}

	int OpenGLFramebuffer::readPixel(uint32_t vAttachmentIndex, int x, int y)
	{
		PK_PROFILE_FUNCTION();

		glReadBuffer(GL_COLOR_ATTACHMENT0 + vAttachmentIndex);
		int PixelData;
		glReadPixels(x, y, 1, 1, GL_RED_INTEGER, GL_INT, &PixelData);
		return PixelData;
	}

	void OpenGLFramebuffer::clearAttachment(uint32_t vAttachmentIndex, int value)
	{
		PK_PROFILE_FUNCTION();
		if (vAttachmentIndex >= m_ColorAttachments.size())
			PK_CORE_ERROR("OpenGLFramebuffer : Invalid color attachment index {0}", vAttachmentIndex);
		glClearTexImage(m_ColorAttachments[vAttachmentIndex], 0,
			Utils::PikaTextureFormatToGLDataFormat(m_ColorAttachmentsSpecification[vAttachmentIndex].m_TextureFormat),
			GL_INT, &value);
	}

}