#include "pkpch.h"
#include "VertexArray.h"
#include "RendererAPI.h"
#include "Pika/Platform/OpenGL/OpenGLVertexArray.h"

namespace Pika
{
	Ref<VertexArray> VertexArray::Create()
	{
		switch (RendererAPI::getAPI())
		{
		case RendererAPI::GraphicsAPI::None:
			PK_CORE_ASSERT(false, "VertexArray: None, PIKA need a graphics API!");
			return nullptr;
#ifdef PK_PLATFORM_WINDOWS
		case RendererAPI::GraphicsAPI::OpenGL:
			return CreateRef<OpenGLVertexArray>();
		case RendererAPI::GraphicsAPI::DirectX:
			PK_CORE_ASSERT(false, "VertexArray: DirectX, PIKA do not support DirectX yet!");
			return nullptr;
#endif
		}
		PK_ASSERT(false, "VertexArray: Unknown renderer API!");
		return nullptr;
	}
}
