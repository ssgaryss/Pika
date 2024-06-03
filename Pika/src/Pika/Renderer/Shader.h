#pragma once
#include <glm/glm.hpp>
#include <unordered_map>

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

		//glm data for now
		virtual void setFloat(const std::string& vName, const float vValue) const = 0;
		virtual void setFloat2(const std::string& vName, const glm::vec2& vValue) const = 0;
		virtual void setFloat3(const std::string& vName, const glm::vec3& vValue) const = 0;
		virtual void setFloat4(const std::string& vName, const glm::vec4& vValue) const = 0;
		virtual void setInt(const std::string& vName, const int vValue) const = 0;
		virtual void setInt2(const std::string& vName, const glm::vec2& vValue) const = 0;
		virtual void setInt3(const std::string& vName, const glm::vec3& vValue) const = 0;
		virtual void setInt4(const std::string& vName, const glm::vec4& vValue) const = 0;
		virtual void setMat3(const std::string& vName, const glm::mat3& vValue) const = 0;
		virtual void setMat4(const std::string& vName, const glm::mat4& vValue) const = 0;
		virtual void setBool(const std::string& vName, const bool vValue) const = 0;

		static Ref<Shader> Create(const std::string& vName, const std::string& vVertexShaderSrc, const std::string& vFragmentShaderSrc);
		static Ref<Shader> Create(const std::string& vFilePath);
	};

	class ShaderLibrary
	{
	public:

	private:
		std::unordered_map<std::string, Ref<Shader>> m_Shaders;
	};

}

