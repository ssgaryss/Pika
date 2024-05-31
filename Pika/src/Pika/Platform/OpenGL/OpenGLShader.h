#pragma once
#include "Pika/Renderer/Shader.h"
#include <string>

namespace Pika
{

	class OpenGLShader : public Shader
	{
	public:
		OpenGLShader(const std::string& vName, const std::string& vVertexShaderSrc, const std::string& vFragmentShaderSrc);
		~OpenGLShader();
		void bind() const override;
		void unbind() const override;
		inline const std::string& getName() const override { return m_Name; }

		void setUniformFloat(const std::string& vName, const float vValue) const override;
		void setUniformFloat2(const std::string& vName, const glm::vec2& vValue) const override;
		void setUniformFloat3(const std::string& vName, const glm::vec3& vValue) const override;
		void setUniformFloat4(const std::string& vName, const glm::vec4& vValue) const override;
		void setUniformInt(const std::string& vName, const int vValue) const override;
		void setUniformInt2(const std::string& vName, const glm::vec2& vValue) const override;
		void setUniformInt3(const std::string& vName, const glm::vec3& vValue) const override;
		void setUniformInt4(const std::string& vName, const glm::vec4& vValue) const override;
		void setUniformMat3(const std::string& vName, const glm::mat3& vValue) const override;
		void setUniformMat4(const std::string& vName, const glm::mat4& vValue) const override;
		void setUniformBool(const std::string& vName, const bool vValue) const override;
	private:
		uint32_t m_RendererID; //shader program ID
		std::string m_Name;

	};

}

