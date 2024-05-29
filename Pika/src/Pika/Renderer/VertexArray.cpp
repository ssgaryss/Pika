#include "pkpch.h"
#include "VertexArray.h"
#include "Pika/Renderer/RendererAPI.h"
#include "Pika/Platform/OpenGL/OpenGLVertexArray.h"

namespace Pika
{
	VertexArray* VertexArray::Create()
	{
		switch (RendererAPI::getAPI())
		{
		case RendererAPI::GraphicsAPI::None:
			PK_CORE_ASSERT(false, "Shader: None, PIKA need a graphics API!");
			return nullptr;
#ifdef PK_PLATFORM_WINDOWS
		case RendererAPI::GraphicsAPI::OpenGL:
			return new OpenGLVertexArray();
		case RendererAPI::GraphicsAPI::DirectX:
			PK_CORE_ASSERT(false, "Shader: DirectX, PIKA do not support DirectX yet!");
			return nullptr;
#endif
		}
		PK_ASSERT(false, "Shader: Unknown renderer API!");
		return nullptr;
	}
}
