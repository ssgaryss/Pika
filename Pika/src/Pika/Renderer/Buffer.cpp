#include "pkpch.h"
#include "Buffer.h"
#include "Pika/Renderer/RendererAPI.h"
#include "Pika/Platform/OpenGL/OpenGLBuffer.h"

namespace Pika {

	////////////////////////////VertexBuffer/////////////////////////////////
	std::shared_ptr<VertexBuffer> VertexBuffer::create(uint32_t vSize)
	{
		switch (RendererAPI::getAPI())
		{
		case RendererAPI::GraphicsAPI::None:
			PK_ASSERT(false, "VertexBuffer : None, PIKA need a graphics API!");
			return nullptr;
#ifdef PK_PLATFORM_WINDOWS
		case RendererAPI::GraphicsAPI::OpenGL:
			return std::make_shared<OpenGLVertexBuffer>(vSize);
		case RendererAPI::GraphicsAPI::DirectX:
			PK_CORE_ASSERT(false, "VertexBuffer: DirectX, PIKA do not support DirectX yet!");
			return nullptr;
#endif
		}

		return nullptr;
	}

	std::shared_ptr<VertexBuffer> VertexBuffer::create(float* vVertices, uint32_t vSize)
	{
		switch (RendererAPI::getAPI())
		{
		case RendererAPI::GraphicsAPI::None:
			PK_ASSERT(false, "VertexBuffer : None, PIKA need a graphics API!");
			return nullptr;
#ifdef PK_PLATFORM_WINDOWS
		case RendererAPI::GraphicsAPI::OpenGL:
			return std::make_shared<OpenGLVertexBuffer>(vVertices, vSize);
		case RendererAPI::GraphicsAPI::DirectX:
			PK_CORE_ASSERT(false, "VertexBuffer: DirectX, PIKA do not support DirectX yet!");
			return nullptr;
#endif
		}
		return nullptr;
	}


	////////////////////////////IndexBuffer/////////////////////////////////
	std::shared_ptr<IndexBuffer> IndexBuffer::create(uint32_t* vIndices, uint32_t vCount)
	{
		switch (RendererAPI::getAPI())
		{
		case RendererAPI::GraphicsAPI::None:
			PK_ASSERT(false, "IndexBuffer : None, PIKA need a graphics API!");
			return nullptr;
#ifdef PK_PLATFORM_WINDOWS
		case RendererAPI::GraphicsAPI::OpenGL:
			return std::make_shared<OpenGLIndexBuffer>(vIndices, vCount);
		case RendererAPI::GraphicsAPI::DirectX:
			PK_CORE_ASSERT(false, "IndexBuffer: DirectX, PIKA do not support DirectX yet!");
			return nullptr;
#endif
		}
		return nullptr;
	}

}
