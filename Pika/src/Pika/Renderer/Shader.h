#pragma once
#include <glm/glm.hpp>

namespace Pika
{
	enum class ShaderDataType
	{
		None = 0, Float, Float2, Float3, Float4, Int, Int2, Int3, Int4, Mat3, Mat4, Bool
	};

	class Shader
	{
	public:
		virtual ~Shader() = default;
		virtual void bind() const = 0;
		virtual void unbind() const = 0;
		virtual const std::string& getName() const = 0;

		virtual void setUniformMat4(const std::string& vName, const glm::mat4& vValue) const = 0;

		static std::shared_ptr<Shader> Create(const std::string& vName,
			const std::string& vVertexShaderSrc, const std::string& vFragmentShaderSrc);
	};

}

