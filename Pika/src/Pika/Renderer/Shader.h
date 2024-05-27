#pragma once

namespace Pika
{

	class Shader
	{
	public:
		virtual ~Shader() = default;
		virtual void bind() = 0;
		virtual void unbind() = 0;
		virtual const std::string& getName() const = 0;

	};

}

