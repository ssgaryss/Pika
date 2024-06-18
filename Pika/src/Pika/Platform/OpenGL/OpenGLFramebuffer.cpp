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
				glTexStorage2D(GL_TEXTURE_2D, 0, vInternalFormat, vWidth, vHeight);
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

	}

	OpenGLFramebuffer::~OpenGLFramebuffer()
	{

	}

	void OpenGLFramebuffer::bind()
	{
		glBindFramebuffer(GL_FRAMEBUFFER, m_RendererID);
	}

	void OpenGLFramebuffer::unbind()
	{
		glBindFramebuffer(GL_FRAMEBUFFER, 0);
	}

	void OpenGLFramebuffer::refresh()
	{
		if (!m_RendererID)
		{
			glDeleteFramebuffers(1, &m_RendererID);
			glDeleteTextures(1, &m_DepthStencilAttachment);
			glDeleteTextures(m_ColoredAttachments.size(), m_ColoredAttachments.data());
			m_DepthStencilAttachment = 0;
			m_ColoredAttachments.clear();
		}

		glCreateFramebuffers(1, &m_RendererID);
		glBindFramebuffer(GL_FRAMEBUFFER, m_RendererID);

	}

}