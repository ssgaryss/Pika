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

		void setViewport(uint32_t vLBX, uint32_t vLBY, uint32_t vRTX, uint32_t vRTY) override;
		void resize(uint32_t vWidth, uint32_t vHeight) override;
		int readPixel(uint32_t vAttachmentIndex, int x, int y) override; // 返回对于位置的红色通道值
		// TODO : readRegion()
		void clearAttachment(uint32_t vAttachmentIndex, int value) override;

		void setDepthStencilAttachment(const Ref<Texture>& vTexture) override;
		void setColorAttachment(uint32_t vIndex, const Ref<Texture>& vTexture) override;
		inline uint32_t getDepthStencilAttachmentRendererID() const override { return m_DepthStencilAttachment; }
		inline uint32_t getColorAttachmentRendererID(uint32_t vIndex = 0) const override { return m_ColorAttachments[vIndex]; }
		inline const FramebufferSpecification& getFramebufferSpecification() const override { return m_Specification; }

	private:
		void checkFramebufferStatus() const;
	private:
		uint32_t m_RendererID = 0;
		FramebufferSpecification m_Specification;

		FramebufferTextureSpecification m_DepthStencilAttachmentSpecification;
		std::vector<FramebufferTextureSpecification> m_ColorAttachmentsSpecification;

		uint32_t m_DepthStencilAttachment = 0; // GL_DEPTH24_STENCIL8
		std::vector<uint32_t> m_ColorAttachments;
	};

}
