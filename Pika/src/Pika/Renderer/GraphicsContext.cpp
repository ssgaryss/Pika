#include "pkpch.h"
#include "GraphicsContext.h"
#include "Pika/Platform/OpenGL/OpenGLContext.h"
#include "Pika/Renderer/RendererAPI.h"


namespace Pika
{
	GraphicsContext* GraphicsContext::create(void* vWindow)
	{
		switch (RendererAPI::getAPI())
		{
		case RendererAPI::GraphicsAPI::None:
			PK_CORE_ASSERT(false, "GraphicsContext: None, PIKA need a graphics API!");
			return nullptr;
#ifdef PK_PLATFORM_WINDOWS
		case RendererAPI::GraphicsAPI::OpenGL:
			return new OpenGLContext(reinterpret_cast<GLFWwindow*>(vWindow));
		case RendererAPI::GraphicsAPI::DirectX:
			PK_CORE_ASSERT(false, "GraphicsContext: DirectX, PIKA do not support DirectX yet!");
			return nullptr;
#endif
		}

		PK_ASSERT(false, "GraphicsContext: Unknown renderer API!");
		return nullptr;
	}
}
