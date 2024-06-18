#pragma once

namespace Pika
{

	enum class FramebufferTextureFormat
	{
		None = 0,
		// Color Attachment
		RGB8,
		RED_INTEGER,
		// Depth/Stencil
		DEPTH24STENCIL8,

		Depth = DEPTH24STENCIL8
	};

	struct FramebufferTextureSpecification
	{
		FramebufferTextureSpecification() = default;
		FramebufferTextureSpecification(FramebufferTextureFormat vFormat)
			: m_TextureFormat{ vFormat } {}
		FramebufferTextureFormat m_TextureFormat = FramebufferTextureFormat::None;
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
		uint32_t m_Width, m_Height;
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

		virtual const FramebufferSpecification& getFramebufferSpecification() const = 0;

		static Ref<Framebuffer> Create(const FramebufferSpecification& vFramebufferSpecifiacation);
	};

}
