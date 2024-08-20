#include "pkpch.h"
#include "Buffer.h"
#include "RendererAPI.h"
#include "Pika/Platform/OpenGL/OpenGLBuffer.h"

namespace Pika {

	////////////////////////////VertexBuffer/////////////////////////////////
	Ref<VertexBuffer> VertexBuffer::Create(uint32_t vSize)
	{
		switch (RendererAPI::getAPI())
		{
		case RendererAPI::GraphicsAPI::None:
			PK_ASSERT(false, "VertexBuffer : None, PIKA need a graphics API!");
			return nullptr;
		case RendererAPI::GraphicsAPI::OpenGL:
			return CreateRef<OpenGLVertexBuffer>(vSize);
#ifdef PK_PLATFORM_WINDOWS
		case RendererAPI::GraphicsAPI::DirectX:
			PK_CORE_ASSERT(false, "VertexBuffer: DirectX, PIKA do not support DirectX yet!");
			return nullptr;
#endif
		}

		return nullptr;
	}

	Ref<VertexBuffer> VertexBuffer::Create(float* vVertices, uint32_t vSize)
	{
		switch (RendererAPI::getAPI())
		{
		case RendererAPI::GraphicsAPI::None:
			PK_ASSERT(false, "VertexBuffer : None, PIKA need a graphics API!");
			return nullptr;
		case RendererAPI::GraphicsAPI::OpenGL:
			return CreateRef<OpenGLVertexBuffer>(vVertices, vSize);
#ifdef PK_PLATFORM_WINDOWS
		case RendererAPI::GraphicsAPI::DirectX:
			PK_CORE_ASSERT(false, "VertexBuffer: DirectX, PIKA do not support DirectX yet!");
			return nullptr;
#endif
		}
		return nullptr;
	}


	////////////////////////////IndexBuffer/////////////////////////////////
	Ref<IndexBuffer> IndexBuffer::Create(uint32_t* vIndices, uint32_t vCount)
	{
		switch (RendererAPI::getAPI())
		{
		case RendererAPI::GraphicsAPI::None:
			PK_ASSERT(false, "IndexBuffer : None, PIKA need a graphics API!");
			return nullptr;
		case RendererAPI::GraphicsAPI::OpenGL:
			return CreateRef<OpenGLIndexBuffer>(vIndices, vCount);
#ifdef PK_PLATFORM_WINDOWS
		case RendererAPI::GraphicsAPI::DirectX:
			PK_CORE_ASSERT(false, "IndexBuffer: DirectX, PIKA do not support DirectX yet!");
			return nullptr;
#endif
		}
		return nullptr;
	}

}
