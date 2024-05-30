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

		void setUniformMat4(const std::string& vName, const glm::mat4& vValue) const override;
	private:
		uint32_t m_RendererID; //shader program ID
		std::string m_Name;

	};

}

