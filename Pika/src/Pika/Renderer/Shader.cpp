#include "pkpch.h"
#include "Shader.h"
#include "RendererAPI.h"
#include "Pika/Platform/OpenGL/OpenGLShader.h"

namespace Pika {

	ShaderSourceMarkers Shader::s_FileMarkers;

	Ref<Shader> Shader::Create(const std::string& vName, const std::string& vVertexShaderSrc, const std::string& vFragmentShaderSrc)
	{
		switch (RendererAPI::getAPI())
		{
		case RendererAPI::GraphicsAPI::None:
			PK_CORE_ASSERT(false, "Shader: None, PIKA need a graphics API!");
			return nullptr;
		case RendererAPI::GraphicsAPI::OpenGL:
			return CreateRef<OpenGLShader>(vName, vVertexShaderSrc, vFragmentShaderSrc);
#ifdef PK_PLATFORM_WINDOWS
		case RendererAPI::GraphicsAPI::DirectX:
			PK_CORE_ASSERT(false, "Shader: DirectX, PIKA do not support DirectX yet!");
			return nullptr;
#endif
		}
		PK_ASSERT(false, "Shader: Unknown renderer API!");
		return nullptr;
	}

	Ref<Shader> Shader::Create(const std::string& vFilePath)
	{
		switch (RendererAPI::getAPI())
		{
		case RendererAPI::GraphicsAPI::None:
			PK_CORE_ASSERT(false, "Shader: None, PIKA need a graphics API!");
			return nullptr;
		case RendererAPI::GraphicsAPI::OpenGL:
			return CreateRef<OpenGLShader>(vFilePath);
#ifdef PK_PLATFORM_WINDOWS
		case RendererAPI::GraphicsAPI::DirectX:
			PK_CORE_ASSERT(false, "Shader: DirectX, PIKA do not support DirectX yet!");
			return nullptr;
#endif
		}
		PK_ASSERT(false, "Shader: Unknown renderer API!");
		return nullptr;
	}

	Ref<Shader> Shader::Create(const std::string& vName, const std::string& vFilePath)
	{
		switch (RendererAPI::getAPI())
		{
		case RendererAPI::GraphicsAPI::None:
			PK_CORE_ASSERT(false, "Shader: None, PIKA need a graphics API!");
			return nullptr;
		case RendererAPI::GraphicsAPI::OpenGL:
			return CreateRef<OpenGLShader>(vName, vFilePath);
#ifdef PK_PLATFORM_WINDOWS
		case RendererAPI::GraphicsAPI::DirectX:
			PK_CORE_ASSERT(false, "Shader: DirectX, PIKA do not support DirectX yet!");
			return nullptr;
#endif
		}
		PK_ASSERT(false, "Shader: Unknown renderer API!");
		return nullptr;
	}

	void ShaderLibrary::add(const Ref<Shader>& vShader)
	{
		std::string Name = vShader->getName();
		add(Name, vShader);
	}

	void ShaderLibrary::add(const std::string& vName, const Ref<Shader>& vShader)
	{
		if (!isExist(vName)) {
			m_Shaders[vName] = vShader;
			vShader->setName(vName);
			vShader->m_InShaderLibrary = true;
		}
		else {
			PK_CORE_WARN(R"(ShaderLibrary : Shader "{0}" is exist!)", vName);
		}
	}

	Ref<Shader> ShaderLibrary::load(const std::string& vFilePath)
	{
		Ref<Shader> pShader = Shader::Create(vFilePath);
		add(pShader);
		return pShader;
	}

	Ref<Shader> ShaderLibrary::load(const std::string& vName, const std::string& vFilePath)
	{
		Ref<Shader> pShader = Shader::Create(vFilePath);
		add(vName, pShader);
		return pShader;
	}

	void ShaderLibrary::remove(const std::string& vName)
	{
		if (!isExist(vName)) {
			PK_CORE_WARN(R"(ShaderLibrary : Shader "{0}" is not exist!)", vName);
			return;
		}
		auto it = m_Shaders.find(vName);
		it->second->m_InShaderLibrary = false;
		m_Shaders.erase(it);
	}

	Ref<Shader> ShaderLibrary::getShader(const std::string& vName)
	{
		if (isExist(vName)) {
			return m_Shaders[vName];
		}
		PK_CORE_WARN(R"(ShaderLibrary : Shader "{0}" is not exist!)", vName);
		return nullptr;
	}

	bool ShaderLibrary::isExist(const std::string& vName) const
	{
		return m_Shaders.find(vName) != m_Shaders.end();
	}

}