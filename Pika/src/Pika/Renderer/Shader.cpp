#include "pkpch.h"
#include "Shader.h"
#include "Pika/Renderer/RendererAPI.h"
#include "Pika/Platform/OpenGL/OpenGLShader.h"

namespace Pika {

	std::shared_ptr<Shader> Shader::Create(const std::string& vName, const std::string& vVertexShaderSrc, const std::string& vFragmentShaderSrc)
	{
		switch (RendererAPI::getAPI())
		{
		case RendererAPI::GraphicsAPI::None:
			PK_CORE_ASSERT(false, "Shader: None, PIKA need a graphics API!");
			return nullptr;
#ifdef PK_PLATFORM_WINDOWS
		case RendererAPI::GraphicsAPI::OpenGL:
			return std::make_shared<OpenGLShader>(vName, vVertexShaderSrc, vFragmentShaderSrc);
		case RendererAPI::GraphicsAPI::DirectX:
			PK_CORE_ASSERT(false, "Shader: DirectX, PIKA do not support DirectX yet!");
			return nullptr;
#endif
		}
		PK_ASSERT(false, "Shader: Unknown renderer API!");
		return nullptr;
	}

}