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

		inline const FramebufferSpecification& getFramebufferSpecification() const { return m_Specification; }

	private:
		uint32_t m_RendererID = 0;
		FramebufferSpecification m_Specification;

		uint32_t m_DepthStencilAttachment = 0; // GL_DEPTH24_STENCIL8
		std::vector<uint32_t> m_ColoredAttachments;
	};

}
