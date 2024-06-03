#include "pkpch.h"
#include "OpenGLShader.h"
#include <glad/glad.h>

namespace Pika
{
	OpenGLShader::OpenGLShader(const std::string& vFilePath)
		: m_FilePath{vFilePath}
	{
		try
		{
			readFile(vFilePath);
		}
		catch (const std::runtime_error& e)
		{
			PK_CORE_ERROR("OpenGLShader : fail to load shader file at {0}", e.what());
		}
	}

	Pika::OpenGLShader::OpenGLShader(const std::string& vName,
		const std::string& vVertexShaderSrc,
		const std::string& vFragmentShaderSrc)
		:m_Name{ vName }
	{
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
		glDeleteProgram(m_RendererID);
	}

	void Pika::OpenGLShader::bind() const
	{
		glUseProgram(m_RendererID);
	}

	void Pika::OpenGLShader::unbind() const
	{
		glUseProgram(0);
	}

	void OpenGLShader::setFloat(const std::string& vName, const float vValue) const
	{
		glUniform1f(glGetUniformLocation(m_RendererID, vName.c_str()), vValue);
	}

	void OpenGLShader::setFloat2(const std::string& vName, const glm::vec2& vValue) const
	{
		glUniform2f(glGetUniformLocation(m_RendererID, vName.c_str()), vValue.x, vValue.y);
	}

	void OpenGLShader::setFloat3(const std::string& vName, const glm::vec3& vValue) const
	{
		glUniform3f(glGetUniformLocation(m_RendererID, vName.c_str()), vValue.x, vValue.y, vValue.z);
	}

	void OpenGLShader::setFloat4(const std::string& vName, const glm::vec4& vValue) const
	{
		glUniform4f(glGetUniformLocation(m_RendererID, vName.c_str()), vValue.x, vValue.y, vValue.z, vValue.w);
	}

	void OpenGLShader::setInt(const std::string& vName, const int vValue) const
	{
		glUniform1i(glGetUniformLocation(m_RendererID, vName.c_str()), vValue);
	}

	void OpenGLShader::setInt2(const std::string& vName, const glm::vec2& vValue) const
	{
		glUniform2i(glGetUniformLocation(m_RendererID, vName.c_str()), (int)vValue.x, (int)vValue.y);
	}

	void OpenGLShader::setInt3(const std::string& vName, const glm::vec3& vValue) const
	{
		glUniform3i(glGetUniformLocation(m_RendererID, vName.c_str()), (int)vValue.x, (int)vValue.y, (int)vValue.z);
	}

	void OpenGLShader::setInt4(const std::string& vName, const glm::vec4& vValue) const
	{
		glUniform4i(glGetUniformLocation(m_RendererID, vName.c_str()), (int)vValue.x, (int)vValue.y, (int)vValue.z, (int)vValue.w);
	}

	void OpenGLShader::setMat3(const std::string& vName, const glm::mat3& vValue) const
	{
		glUniformMatrix3fv(glGetUniformLocation(m_RendererID, vName.c_str()), 1, GL_FALSE, &vValue[0][0]);
	}

	void OpenGLShader::setMat4(const std::string& vName, const glm::mat4& vValue) const
	{
		glUniformMatrix4fv(glGetUniformLocation(m_RendererID, vName.c_str()), 1, GL_FALSE, &vValue[0][0]);
	}

	void OpenGLShader::setBool(const std::string& vName, const bool vValue) const
	{
		glUniform1i(glGetUniformLocation(m_RendererID, vName.c_str()), vValue);
	}

	void OpenGLShader::compileAndLinkShader(const std::string& vVertexShaderSrc, const std::string& vFragmentShaderSrc)
	{
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

	std::string OpenGLShader::readFile(const std::string& vPath)
	{
		std::string Result;
		std::ifstream in(vPath, std::ios::in | std::ios::binary);
		if (!in.good()) {
			throw std::runtime_error("Path : " + vPath);
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
		return Result;
	}

}
