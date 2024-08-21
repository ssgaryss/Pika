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
			PK_CORE_ASSERT(false, "Texture2D: None, PIKA need a graphics API!");
			return nullptr;
		case RendererAPI::GraphicsAPI::OpenGL:
			return CreateRef<OpenGLTexture2D>(vTextureSpecification);
#ifdef PK_PLATFORM_WINDOWS
		case RendererAPI::GraphicsAPI::DirectX:
			PK_CORE_ASSERT(false, "Texture2D: DirectX, PIKA do not support DirectX yet!");
			return nullptr;
#endif
		}
		PK_ASSERT(false, "Texture2D: Unknown renderer API!");
		return nullptr;
	}

	Ref<Texture2D> Texture2D::Create(const std::filesystem::path& vPath, bool vRequiredMips)
	{
		switch (RendererAPI::getAPI())
		{
		case RendererAPI::GraphicsAPI::None:
			PK_CORE_ASSERT(false, "Texture2D: None, PIKA need a graphics API!");
			return nullptr;
		case RendererAPI::GraphicsAPI::OpenGL:
			return CreateRef<OpenGLTexture2D>(vPath, vRequiredMips);
#ifdef PK_PLATFORM_WINDOWS
		case RendererAPI::GraphicsAPI::DirectX:
			PK_CORE_ASSERT(false, "Texture2D: DirectX, PIKA do not support DirectX yet!");
			return nullptr;
#endif
		}
		PK_ASSERT(false, "Texture2D: Unknown renderer API!");
		return nullptr;
	}

	Ref<Cubemap> Cubemap::Create(const std::filesystem::path& vPath, bool vRequiredMips)
	{
		switch (RendererAPI::getAPI())
		{
		case RendererAPI::GraphicsAPI::None:
			PK_CORE_ASSERT(false, "Cubemap: None, PIKA need a graphics API!");
			return nullptr;
		case RendererAPI::GraphicsAPI::OpenGL:
			return CreateRef<OpenGLCubemap>(vPath, vRequiredMips);
#ifdef PK_PLATFORM_WINDOWS
		case RendererAPI::GraphicsAPI::DirectX:
			PK_CORE_ASSERT(false, "Cubemap: DirectX, PIKA do not support DirectX yet!");
			return nullptr;
#endif
		}
		PK_ASSERT(false, "Cubemap: Unknown renderer API!");
		return nullptr;
	}

}