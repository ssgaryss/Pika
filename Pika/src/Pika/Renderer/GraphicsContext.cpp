#include "pkpch.h"
#include "GraphicsContext.h"
#include "RendererAPI.h"
#include "Pika/Platform/OpenGL/OpenGLContext.h"


namespace Pika
{
	Ref<GraphicsContext> GraphicsContext::Create(void* vWindow)
	{
		switch (RendererAPI::getAPI())
		{
		case RendererAPI::GraphicsAPI::None:
			PK_CORE_ASSERT(false, "GraphicsContext: None, PIKA need a graphics API!");
			return nullptr;
		case RendererAPI::GraphicsAPI::OpenGL:
			return CreateRef<OpenGLContext>(reinterpret_cast<GLFWwindow*>(vWindow));
#ifdef PK_PLATFORM_WINDOWS
		case RendererAPI::GraphicsAPI::DirectX:
			PK_CORE_ASSERT(false, "GraphicsContext: DirectX, PIKA do not support DirectX yet!");
			return nullptr;
#endif
		}

		PK_ASSERT(false, "GraphicsContext: Unknown renderer API!");
		return nullptr;
	}
}
