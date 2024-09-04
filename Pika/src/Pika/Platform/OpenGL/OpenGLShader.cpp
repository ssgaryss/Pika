#include "pkpch.h"
#include "OpenGLShader.h"
#include <glad/glad.h>

namespace Pika
{
	OpenGLShader::OpenGLShader(const std::filesystem::path& vFilePath)
		: m_FilePath{ vFilePath }
	{
		PK_PROFILE_FUNCTION();

		try
		{
			std::string Source = readFile(vFilePath);
			m_OpenGLSourceCode = preProcess(Source);
			compileAndLinkShader(m_OpenGLSourceCode[GL_VERTEX_SHADER], m_OpenGLSourceCode[GL_FRAGMENT_SHADER]);
		}
		catch (const std::runtime_error& e)
		{
			PK_CORE_WARN("OpenGLShader : Fail to load shader file!\n {0}", e.what());
		}
	}

	OpenGLShader::OpenGLShader(const std::string& vName, const std::filesystem::path& vFilePath)
		: OpenGLShader{ vFilePath }
	{
		m_Name = vName;
	}

	Pika::OpenGLShader::OpenGLShader(const std::string& vName,
		const std::string& vVertexShaderSrc,
		const std::string& vFragmentShaderSrc)
		:m_Name{ vName }
	{
		PK_PROFILE_FUNCTION();

		try
		{
			compileAndLinkShader(vVertexShaderSrc, vFragmentShaderSrc);
		}
		catch (const std::runtime_error& e)
		{
			std::cerr << e.what() << std::endl;
			return;
		}
	}

	Pika::OpenGLShader::~OpenGLShader()
	{
		PK_PROFILE_FUNCTION();

		glDeleteProgram(m_RendererID);
	}

	void Pika::OpenGLShader::bind() const
	{
		PK_PROFILE_FUNCTION();

		glUseProgram(m_RendererID);
	}

	void Pika::OpenGLShader::unbind() const
	{
		PK_PROFILE_FUNCTION();

		glUseProgram(0);
	}

	void OpenGLShader::setName(const std::string& vName)
	{
		if (!m_InShaderLibrary)
			m_Name = vName;
		else
			PK_CORE_WARN(R"(OpenGLShader : Shader "{0}" is already in ShaderLibrary which is not allow to change name!)", m_Name);
	}

	void OpenGLShader::setFloat(const std::string& vName, const float vValue) const
	{
		PK_PROFILE_FUNCTION();
		GLint Location = glGetUniformLocation(m_RendererID, vName.c_str());
		if (Location == -1) {
			PK_CORE_WARN(R"(OpenGLShader : Can not find variable "{0} in shader "{1}"!")", vName, m_Name);
			return;
		}
		glUniform1f(Location, vValue);
	}

	void OpenGLShader::setFloat2(const std::string& vName, const glm::vec2& vValue) const
	{
		PK_PROFILE_FUNCTION();
		GLint Location = glGetUniformLocation(m_RendererID, vName.c_str());
		if (Location == -1) {
			PK_CORE_WARN(R"(OpenGLShader : Can not find variable "{0} in shader "{1}"!")", vName, m_Name);
			return;
		}
		glUniform2f(Location, vValue.x, vValue.y);
	}

	void OpenGLShader::setFloat3(const std::string& vName, const glm::vec3& vValue) const
	{
		PK_PROFILE_FUNCTION();
		GLint Location = glGetUniformLocation(m_RendererID, vName.c_str());
		if (Location == -1) {
			PK_CORE_WARN(R"(OpenGLShader : Can not find variable "{0} in shader "{1}"!")", vName, m_Name);
			return;
		}
		glUniform3f(Location, vValue.x, vValue.y, vValue.z);
	}

	void OpenGLShader::setFloat4(const std::string& vName, const glm::vec4& vValue) const
	{
		PK_PROFILE_FUNCTION();
		GLint Location = glGetUniformLocation(m_RendererID, vName.c_str());
		if (Location == -1) {
			PK_CORE_WARN(R"(OpenGLShader : Can not find variable "{0} in shader "{1}"!")", vName, m_Name);
			return;
		}
		glUniform4f(Location, vValue.x, vValue.y, vValue.z, vValue.w);
	}

	void OpenGLShader::setInt(const std::string& vName, const int vValue) const
	{
		PK_PROFILE_FUNCTION();
		GLint Location = glGetUniformLocation(m_RendererID, vName.c_str());
		if (Location == -1) {
			PK_CORE_WARN(R"(OpenGLShader : Can not find variable "{0} in shader "{1}"!")", vName, m_Name);
			return;
		}
		glUniform1i(Location, vValue);
	}

	void OpenGLShader::setInt2(const std::string& vName, const glm::vec2& vValue) const
	{
		PK_PROFILE_FUNCTION();
		GLint Location = glGetUniformLocation(m_RendererID, vName.c_str());
		if (Location == -1) {
			PK_CORE_WARN(R"(OpenGLShader : Can not find variable "{0} in shader "{1}"!")", vName, m_Name);
			return;
		}
		glUniform2i(Location, (int)vValue.x, (int)vValue.y);
	}

	void OpenGLShader::setInt3(const std::string& vName, const glm::vec3& vValue) const
	{
		PK_PROFILE_FUNCTION();
		GLint Location = glGetUniformLocation(m_RendererID, vName.c_str());
		if (Location == -1) {
			PK_CORE_WARN(R"(OpenGLShader : Can not find variable "{0} in shader "{1}"!")", vName, m_Name);
			return;
		}
		glUniform3i(Location, (int)vValue.x, (int)vValue.y, (int)vValue.z);
	}

	void OpenGLShader::setInt4(const std::string& vName, const glm::vec4& vValue) const
	{
		PK_PROFILE_FUNCTION();
		GLint Location = glGetUniformLocation(m_RendererID, vName.c_str());
		if (Location == -1) {
			PK_CORE_WARN(R"(OpenGLShader : Can not find variable "{0} in shader "{1}"!")", vName, m_Name);
			return;
		}
		glUniform4i(Location, (int)vValue.x, (int)vValue.y, (int)vValue.z, (int)vValue.w);
	}

	void OpenGLShader::setMat3(const std::string& vName, const glm::mat3& vValue) const
	{
		PK_PROFILE_FUNCTION();
		GLint Location = glGetUniformLocation(m_RendererID, vName.c_str());
		if (Location == -1) {
			PK_CORE_WARN(R"(OpenGLShader : Can not find variable "{0} in shader "{1}"!")", vName, m_Name);
			return;
		}
		glUniformMatrix3fv(Location, 1, GL_FALSE, &vValue[0][0]);
	}

	void OpenGLShader::setMat4(const std::string& vName, const glm::mat4& vValue) const
	{
		PK_PROFILE_FUNCTION();
		GLint Location = glGetUniformLocation(m_RendererID, vName.c_str());
		if (Location == -1) {
			PK_CORE_WARN(R"(OpenGLShader : Can not find variable "{0} in shader "{1}"!")", vName, m_Name);
			return;
		}
		glUniformMatrix4fv(Location, 1, GL_FALSE, &vValue[0][0]);
	}

	void OpenGLShader::setBool(const std::string& vName, const bool vValue) const
	{
		PK_PROFILE_FUNCTION();
		GLint Location = glGetUniformLocation(m_RendererID, vName.c_str());
		if (Location == -1) {
			PK_CORE_WARN(R"(OpenGLShader : Can not find variable "{0} in shader "{1}"!")", vName, m_Name);
			return;
		}
		glUniform1i(Location, vValue);
	}

	void OpenGLShader::setIntArray(const std::string& vName, const int* vValue, uint32_t vCount) const
	{
		PK_PROFILE_FUNCTION();
		GLint Location = glGetUniformLocation(m_RendererID, vName.c_str());
		if (Location == -1) {
			PK_CORE_WARN(R"(OpenGLShader : Can not find variable "{0} in shader "{1}"!")", vName, m_Name);
			return;
		}
		glUniform1iv(Location, vCount, vValue);
	}

	void OpenGLShader::compileAndLinkShader(const std::string& vVertexShaderSrc, const std::string& vFragmentShaderSrc)
	{
		PK_PROFILE_FUNCTION();

		GLuint VertexShader = glCreateShader(GL_VERTEX_SHADER);
		const GLchar* Source = (const GLchar*)vVertexShaderSrc.c_str();
		glShaderSource(VertexShader, 1, &Source, 0);
		glCompileShader(VertexShader);
		GLint IsCompiled = 0;
		glGetShaderiv(VertexShader, GL_COMPILE_STATUS, &IsCompiled);
		if (IsCompiled == GL_FALSE)
		{
			PK_CORE_ERROR("Shader : {0}'s vertex part fail to compile!", m_Name);
			GLint maxLength = 0;
			glGetShaderiv(VertexShader, GL_INFO_LOG_LENGTH, &maxLength);
			std::vector<GLchar> InfoLog(maxLength);
			glGetShaderInfoLog(VertexShader, maxLength, &maxLength, &InfoLog[0]);
			glDeleteShader(VertexShader);
			const std::string Information(InfoLog.begin(), InfoLog.end());
			throw std::runtime_error(Information);
		}

		GLuint FragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
		Source = (const GLchar*)vFragmentShaderSrc.c_str();
		glShaderSource(FragmentShader, 1, &Source, 0);
		glCompileShader(FragmentShader);
		glGetShaderiv(FragmentShader, GL_COMPILE_STATUS, &IsCompiled);
		if (IsCompiled == GL_FALSE)
		{
			PK_CORE_ERROR("Shader : {0}'s fragment part fail to compile!", m_Name);
			GLint maxLength = 0;
			glGetShaderiv(FragmentShader, GL_INFO_LOG_LENGTH, &maxLength);
			std::vector<GLchar> InfoLog(maxLength);
			glGetShaderInfoLog(FragmentShader, maxLength, &maxLength, &InfoLog[0]);
			glDeleteShader(FragmentShader);
			glDeleteShader(VertexShader);
			const std::string Information(InfoLog.begin(), InfoLog.end());
			throw std::runtime_error(Information);
		}

		m_RendererID = glCreateProgram();
		glAttachShader(m_RendererID, VertexShader);
		glAttachShader(m_RendererID, FragmentShader);
		glLinkProgram(m_RendererID);
		GLint IsLinked = 0;
		glGetProgramiv(m_RendererID, GL_LINK_STATUS, (int*)&IsLinked);
		if (IsLinked == GL_FALSE)
		{
			PK_CORE_ERROR("Shader : {0} fail to link!", m_Name);
			GLint maxLength = 0;
			glGetProgramiv(m_RendererID, GL_INFO_LOG_LENGTH, &maxLength);
			std::vector<GLchar> InfoLog(maxLength);
			glGetProgramInfoLog(m_RendererID, maxLength, &maxLength, &InfoLog[0]);
			glDeleteProgram(m_RendererID);
			glDeleteShader(VertexShader);
			glDeleteShader(FragmentShader);
			const std::string Information(InfoLog.begin(), InfoLog.end());
			throw std::runtime_error(Information);
		}
		glDetachShader(m_RendererID, VertexShader);
		glDetachShader(m_RendererID, FragmentShader);
	}

	std::string OpenGLShader::readFile(const std::filesystem::path& vPath)
	{
		PK_PROFILE_FUNCTION();

		if (!vPath.empty() && vPath.has_filename() && vPath.has_extension()) {
			m_Name = vPath.stem().string();
		}
		std::string Result;
		std::ifstream in(vPath, std::ios::in | std::ios::binary);
		if (!in.good()) {
			throw std::runtime_error("Path : " + vPath.string());
		}
		in.seekg(0, std::ios_base::end);
		size_t Size = in.tellg();
		if (Size) {
			Result.resize(Size);
			in.seekg(0, std::ios_base::beg);
			in.read(&Result[0], Size);
		}
		else {
			PK_CORE_INFO("OpenGLShader : load an empty shader file!");
		}
		PK_CORE_INFO("OpenGLShader : Success to load a shader at {0}.", vPath.string());
		return Result;
	}

	std::unordered_map<GLenum, std::string> OpenGLShader::preProcess(const std::string& vSources)
	{
		PK_PROFILE_FUNCTION();

		std::unordered_map<GLenum, std::string> ShaderSources;
		//vertex shader
		size_t Begin = vSources.find(Shader::s_FileMarkers.VertexShaderBegin);
		size_t End = vSources.find(Shader::s_FileMarkers.VertexShaderEnd);
		if (Begin != std::string::npos && End != std::string::npos && Begin < End) {
			Begin += Shader::s_FileMarkers.VertexShaderBegin.length();
			ShaderSources[GL_VERTEX_SHADER] = vSources.substr(Begin, End - Begin);
			PK_CORE_TRACE("GL_VERTEX_SHADER content: {}", ShaderSources[GL_VERTEX_SHADER]);
		}
		//fragment shader
		Begin = vSources.find(Shader::s_FileMarkers.FragmentShaderBegin);
		End = vSources.find(Shader::s_FileMarkers.FragmentShaderEnd);
		if (Begin != std::string::npos && End != std::string::npos && Begin < End) {
			Begin += Shader::s_FileMarkers.FragmentShaderBegin.length();
			ShaderSources[GL_FRAGMENT_SHADER] = vSources.substr(Begin, End - Begin);
			PK_CORE_TRACE("GL_FRAGMENT_SHADER content:{}", ShaderSources[GL_FRAGMENT_SHADER]);
		}
		//pixel shader
		Begin = vSources.find(Shader::s_FileMarkers.PixelShaderBegin);
		End = vSources.find(Shader::s_FileMarkers.PixelShaderEnd);
		if (Begin != std::string::npos && End != std::string::npos && Begin < End) {
			Begin += Shader::s_FileMarkers.PixelShaderBegin.length();
			ShaderSources[GL_FRAGMENT_SHADER] = vSources.substr(Begin, End - Begin);
			PK_CORE_TRACE("{}", ShaderSources[GL_FRAGMENT_SHADER]);
		}

		return ShaderSources;
	}

}
