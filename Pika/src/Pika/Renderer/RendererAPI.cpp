#include "pkpch.h"
#include "RendererAPI.h"
#include "Pika/Platform/OpenGL/OpenGLRendererAPI.h"

namespace Pika {

	RendererAPI::GraphicsAPI RendererAPI::s_GraphicsAPI = GraphicsAPI::OpenGL; //for now

	Scope<RendererAPI> RendererAPI::Create()
	{
		switch (s_GraphicsAPI)
		{
		case Pika::RendererAPI::GraphicsAPI::None:
			PK_ASSERT(false, "RendererAPI: None, PIKA need a graphics API!");
			return nullptr;
#ifdef PK_PLATFORM_WINDOWS
		case Pika::RendererAPI::GraphicsAPI::OpenGL:
			return CreateScope<OpenGLRendererAPI>();
		case Pika::RendererAPI::GraphicsAPI::DirectX:
			PK_ASSERT(false, "RendererAPI: DirectX, PIKA do not support DirectX yet!");
			return nullptr;
#endif
		}

		PK_ASSERT(false, "RendererAPI: Unknown renderer API!");
		return nullptr;
	}

}