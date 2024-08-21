#include "pkpch.h"
#include "Framebuffer.h"
#include "RendererAPI.h"
#include "Pika/Platform/OpenGL/OpenGLFramebuffer.h"

namespace Pika {

    Ref<Framebuffer> Framebuffer::Create(const FramebufferSpecification& vFramebufferSpecifiacation)
    {
		switch (RendererAPI::getAPI())
		{
		case RendererAPI::GraphicsAPI::None:
			PK_CORE_ASSERT(false, "Framebuffer: None, PIKA need a graphics API!");
			return nullptr;
		case RendererAPI::GraphicsAPI::OpenGL:
			return CreateRef<OpenGLFramebuffer>(vFramebufferSpecifiacation);
#ifdef PK_PLATFORM_WINDOWS
		case RendererAPI::GraphicsAPI::DirectX:
			PK_CORE_ASSERT(false, "Framebuffer: DirectX, PIKA do not support DirectX yet!");
			return nullptr;
#endif
		}
		PK_ASSERT(false, "Framebuffer: Unknown renderer API!");
		return nullptr;
    }

}