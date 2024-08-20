#include "pkpch.h"
#include "UniformBuffer.h"
#include "RendererAPI.h"
#include "Pika/Platform/OpenGL/OpenGLUniformBuffer.h"

namespace Pika {

	Ref<UniformBuffer> UniformBuffer::Create(uint32_t vSize, uint32_t vBinding)
	{
		switch (RendererAPI::getAPI())
		{
		case RendererAPI::GraphicsAPI::None:
			PK_CORE_ASSERT(false, "UniformBuffer: None, PIKA need a graphics API!");
			return nullptr;
		case RendererAPI::GraphicsAPI::OpenGL:
			return CreateRef<OpenGLUniformBuffer>(vSize, vBinding);
#ifdef PK_PLATFORM_WINDOWS
		case RendererAPI::GraphicsAPI::DirectX:
			PK_CORE_ASSERT(false, "UniformBuffer: DirectX, PIKA do not support DirectX yet!");
			return nullptr;
#endif
		}
		PK_ASSERT(false, "UniformBuffer: Unknown renderer API!");
		return nullptr;
	}

}