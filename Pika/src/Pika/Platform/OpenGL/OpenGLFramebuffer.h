#pragma once
#include "Pika/Renderer/Framebuffer.h"

namespace Pika
{

	class OpenGLFramebuffer : public Framebuffer
	{
	public:
		OpenGLFramebuffer(const FramebufferSpecification& vFramebufferSpecification);
		~OpenGLFramebuffer();

		void bind() override;
		void unbind() override;
		void refresh() override;

		inline uint32_t getColorAttachmentRendererID(uint32_t vIndex = 0) const { return m_ColorAttachments[vIndex]; }
		inline const FramebufferSpecification& getFramebufferSpecification() const { return m_Specification; }

	private:
		uint32_t m_RendererID = 0;
		FramebufferSpecification m_Specification;

		FramebufferTextureSpecification m_DepthStencilAttachmentSpecification;
		std::vector<FramebufferTextureSpecification> m_ColorAttachmentsSpecification;

		uint32_t m_DepthStencilAttachment = 0; // GL_DEPTH24_STENCIL8
		std::vector<uint32_t> m_ColorAttachments;
	};

}
