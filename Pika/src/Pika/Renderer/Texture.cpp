#include "pkpch.h"
#include "Texture.h"
#include "RendererAPI.h"
#include "Pika/Platform/OpenGL/OpenGLTexture.h"

namespace Pika {
	Ref<Texture2D> Texture2D::Create(const TextureSpecification& vTextureSpecification)
	{
		switch (RendererAPI::getAPI())
		{
		case RendererAPI::GraphicsAPI::None:
			PK_CORE_ASSERT(false, "Shader: None, PIKA need a graphics API!");
			return nullptr;
#ifdef PK_PLATFORM_WINDOWS
		case RendererAPI::GraphicsAPI::OpenGL:
			return CreateRef<OpenGLTexture2D>(vTextureSpecification);
		case RendererAPI::GraphicsAPI::DirectX:
			PK_CORE_ASSERT(false, "Shader: DirectX, PIKA do not support DirectX yet!");
			return nullptr;
#endif
		}
		PK_ASSERT(false, "Shader: Unknown renderer API!");
		return nullptr;
	}


	Ref<Texture2D> Texture2D::Create(const std::string& vPath)
	{
		switch (RendererAPI::getAPI())
		{
		case RendererAPI::GraphicsAPI::None:
			PK_CORE_ASSERT(false, "Shader: None, PIKA need a graphics API!");
			return nullptr;
#ifdef PK_PLATFORM_WINDOWS
		case RendererAPI::GraphicsAPI::OpenGL:
			return CreateRef<OpenGLTexture2D>(vPath);
		case RendererAPI::GraphicsAPI::DirectX:
			PK_CORE_ASSERT(false, "Shader: DirectX, PIKA do not support DirectX yet!");
			return nullptr;
#endif
		}
		PK_ASSERT(false, "Shader: Unknown renderer API!");
		return nullptr;
	}

}