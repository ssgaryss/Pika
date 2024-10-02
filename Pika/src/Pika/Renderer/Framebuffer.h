#pragma once
#include "TextureFormat.h"

namespace Pika
{

	struct FramebufferTextureSpecification
	{
		FramebufferTextureSpecification() = default;
		FramebufferTextureSpecification(TextureFormat vFormat)
			: m_TextureFormat{ vFormat } {}
		TextureFormat m_TextureFormat = TextureFormat::None;
	};

	struct FramebufferAttachmentSpecification
	{
		FramebufferAttachmentSpecification() = default;
		FramebufferAttachmentSpecification(std::initializer_list<FramebufferTextureSpecification> vAttachments)
			: m_Attachments{ vAttachments } {}
		std::vector<FramebufferTextureSpecification> m_Attachments;
	};

	struct  FramebufferSpecification
	{
		uint32_t m_Width = 0, m_Height = 0;
		uint32_t m_Samples = 1; // MSAA sample points nums (can be 2, 4, 8 or 16 ...)
		FramebufferAttachmentSpecification m_FramebufferAttachments;
		bool m_IsSwapChainTarget = false;
	};


	class Framebuffer
	{
	public:
		virtual ~Framebuffer() = default;

		virtual void bind() = 0;
		virtual void unbind() = 0;
		virtual void refresh() = 0; // Regenerate the whole framebuffer

		virtual void setViewport(uint32_t vLBX, uint32_t vLBY, uint32_t vRTX, uint32_t vRTY) = 0;
		virtual void resize(uint32_t vWidth, uint32_t vHeight) = 0;
		virtual int readPixel(uint32_t vAttachmentIndex, int x, int y) = 0;
		virtual void clearAttachment(uint32_t vAttachmentIndex, int value) = 0;

		virtual uint32_t getDepthStencilAttachmentRendererID() const = 0;
		virtual uint32_t getColorAttachmentRendererID(uint32_t vIndex = 0) const = 0;
		virtual const FramebufferSpecification& getFramebufferSpecification() const = 0;

		static Ref<Framebuffer> Create(const FramebufferSpecification& vFramebufferSpecifiacation);
	};

}
