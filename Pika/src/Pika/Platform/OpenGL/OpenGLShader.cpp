#include "pkpch.h"
#include "OpenGLShader.h"
#include <glad/glad.h>

namespace Pika
{

	Pika::OpenGLShader::OpenGLShader(const std::string& vName,
		const std::string& vVertexShaderSrc,
		const std::string& vFragmentShaderSrc)
		:m_Name{ vName }
	{
		// Create an empty vertex shader handle
		GLuint vertexShader = glCreateShader(GL_VERTEX_SHADER);

		// Send the vertex shader source code to GL
		// Note that std::string's .c_str is NULL character terminated.
		const GLchar* source = (const GLchar*)vVertexShaderSrc.c_str();
		glShaderSource(vertexShader, 1, &source, 0);

		// Compile the vertex shader
		glCompileShader(vertexShader);

		GLint isCompiled = 0;
		glGetShaderiv(vertexShader, GL_COMPILE_STATUS, &isCompiled);
		if (isCompiled == GL_FALSE)
		{
			GLint maxLength = 0;
			glGetShaderiv(vertexShader, GL_INFO_LOG_LENGTH, &maxLength);

			// The maxLength includes the NULL character
			std::vector<GLchar> infoLog(maxLength);
			glGetShaderInfoLog(vertexShader, maxLength, &maxLength, &infoLog[0]);

			// We don't need the shader anymore.
			glDeleteShader(vertexShader);

			// Use the infoLog as you see fit.

			// In this simple program, we'll just leave
			return;
		}

		// Create an empty fragment shader handle
		GLuint fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);

		// Send the fragment shader source code to GL
		// Note that std::string's .c_str is NULL character terminated.
		source = (const GLchar*)vFragmentShaderSrc.c_str();
		glShaderSource(fragmentShader, 1, &source, 0);

		// Compile the fragment shader
		glCompileShader(fragmentShader);

		glGetShaderiv(fragmentShader, GL_COMPILE_STATUS, &isCompiled);
		if (isCompiled == GL_FALSE)
		{
			GLint maxLength = 0;
			glGetShaderiv(fragmentShader, GL_INFO_LOG_LENGTH, &maxLength);

			// The maxLength includes the NULL character
			std::vector<GLchar> infoLog(maxLength);
			glGetShaderInfoLog(fragmentShader, maxLength, &maxLength, &infoLog[0]);

			// We don't need the shader anymore.
			glDeleteShader(fragmentShader);
			// Either of them. Don't leak shaders.
			glDeleteShader(vertexShader);

			// Use the infoLog as you see fit.

			// In this simple program, we'll just leave
			return;
		}

		// Vertex and fragment shaders are successfully compiled.
		// Now time to link them together into a program.
		// Get a program object.
		m_RendererID = glCreateProgram();

		// Attach our shaders to our program
		glAttachShader(m_RendererID, vertexShader);
		glAttachShader(m_RendererID, fragmentShader);

		// Link our program
		glLinkProgram(m_RendererID);

		// Note the different functions here: glGetProgram* instead of glGetShader*.
		GLint isLinked = 0;
		glGetProgramiv(m_RendererID, GL_LINK_STATUS, (int*)&isLinked);
		if (isLinked == GL_FALSE)
		{
			GLint maxLength = 0;
			glGetProgramiv(m_RendererID, GL_INFO_LOG_LENGTH, &maxLength);

			// The maxLength includes the NULL character
			std::vector<GLchar> infoLog(maxLength);
			glGetProgramInfoLog(m_RendererID, maxLength, &maxLength, &infoLog[0]);

			// We don't need the program anymore.
			glDeleteProgram(m_RendererID);
			// Don't leak shaders either.
			glDeleteShader(vertexShader);
			glDeleteShader(fragmentShader);

			// Use the infoLog as you see fit.

			// In this simple program, we'll just leave
			return;
		}

		// Always detach shaders after a successful link.
		glDetachShader(m_RendererID, vertexShader);
		glDetachShader(m_RendererID, fragmentShader);
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

	void OpenGLShader::setUniformFloat(const std::string& vName, const float vValue) const
	{
		glUniform1f(glGetUniformLocation(m_RendererID, vName.c_str()), vValue);
	}

	void OpenGLShader::setUniformFloat2(const std::string& vName, const glm::vec2& vValue) const
	{
		glUniform2f(glGetUniformLocation(m_RendererID, vName.c_str()), vValue.x, vValue.y);
	}

	void OpenGLShader::setUniformFloat3(const std::string& vName, const glm::vec3& vValue) const
	{
		glUniform3f(glGetUniformLocation(m_RendererID, vName.c_str()), vValue.x, vValue.y, vValue.z);
	}

	void OpenGLShader::setUniformFloat4(const std::string& vName, const glm::vec4& vValue) const
	{
		glUniform4f(glGetUniformLocation(m_RendererID, vName.c_str()), vValue.x, vValue.y, vValue.z, vValue.w);
	}

	void OpenGLShader::setUniformInt(const std::string& vName, const int vValue) const
	{
		glUniform1i(glGetUniformLocation(m_RendererID, vName.c_str()), vValue);
	}

	void OpenGLShader::setUniformInt2(const std::string& vName, const glm::vec2& vValue) const
	{
		glUniform2i(glGetUniformLocation(m_RendererID, vName.c_str()), (int)vValue.x, (int)vValue.y);
	}

	void OpenGLShader::setUniformInt3(const std::string& vName, const glm::vec3& vValue) const
	{
		glUniform3i(glGetUniformLocation(m_RendererID, vName.c_str()), (int)vValue.x, (int)vValue.y, (int)vValue.z);
	}

	void OpenGLShader::setUniformInt4(const std::string& vName, const glm::vec4& vValue) const
	{
		glUniform4i(glGetUniformLocation(m_RendererID, vName.c_str()), (int)vValue.x, (int)vValue.y, (int)vValue.z, (int)vValue.w);
	}

	void OpenGLShader::setUniformMat3(const std::string& vName, const glm::mat3& vValue) const
	{
		glUniformMatrix3fv(glGetUniformLocation(m_RendererID, vName.c_str()), 1, GL_FALSE, &vValue[0][0]);
	}

	void OpenGLShader::setUniformMat4(const std::string& vName, const glm::mat4& vValue) const
	{
		glUniformMatrix4fv(glGetUniformLocation(m_RendererID, vName.c_str()), 1, GL_FALSE, &vValue[0][0]);
	}

	void OpenGLShader::setUniformBool(const std::string& vName, const bool vValue) const
	{
		glUniform1i(glGetUniformLocation(m_RendererID, vName.c_str()), vValue);
	}

}
