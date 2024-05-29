#pragma once

namespace Pika
{

	class Shader
	{
	public:
		virtual ~Shader() = default;
		virtual void bind() const = 0;
		virtual void unbind() const = 0;
		virtual const std::string& getName() const = 0;

		static std::shared_ptr<Shader> Create(const std::string& vName, 
			const std::string& vVertexShaderSrc, const std::string& vFragmentShaderSrc);
	};

}

