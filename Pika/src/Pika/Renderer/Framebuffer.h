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


	class Framebuffer
	{
	public:
		virtual ~Framebuffer() = default;

		virtual void bind() = 0;
		virtual void unbind() = 0;

		static Ref<Framebuffer> Create();
	private:
		 
	};

}
