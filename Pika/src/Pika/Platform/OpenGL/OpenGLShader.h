#pragma once
#include "Pika/Renderer/Shader.h"
#include <string>
#include <unordered_map>
#include <glad/glad.h>

namespace Pika
{

	class OpenGLShader : public Shader
	{
	public:
		OpenGLShader(const std::string& vFilePath);
		OpenGLShader(const std::string& vName, const std::string& vVertexShaderSrc, const std::string& vFragmentShaderSrc);
		~OpenGLShader();
		void bind() const override;
		void unbind() const override;
		inline const std::string& getName() const override { return m_Name; }

		void setFloat(const std::string& vName, const float vValue) const override;
		void setFloat2(const std::string& vName, const glm::vec2& vValue) const override;
		void setFloat3(const std::string& vName, const glm::vec3& vValue) const override;
		void setFloat4(const std::string& vName, const glm::vec4& vValue) const override;
		void setInt(const std::string& vName, const int vValue) const override;
		void setInt2(const std::string& vName, const glm::vec2& vValue) const override;
		void setInt3(const std::string& vName, const glm::vec3& vValue) const override;
		void setInt4(const std::string& vName, const glm::vec4& vValue) const override;
		void setMat3(const std::string& vName, const glm::mat3& vValue) const override;
		void setMat4(const std::string& vName, const glm::mat4& vValue) const override;
		void setBool(const std::string& vName, const bool vValue) const override;
	private:
		void compileAndLinkShader(const std::string& vVertexShaderSrc, const std::string& vFragmentShaderSrc);
		std::string readFile(const std::string& vPath);
	private:
		uint32_t m_RendererID; //shader program ID
		std::string m_Name;
		std::string m_FilePath;
		std::unordered_map<GLenum, std::string> m_OpenGLSourceCode;

	};

}

