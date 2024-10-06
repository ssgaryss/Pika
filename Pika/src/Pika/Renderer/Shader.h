#pragma once
#include <glm/glm.hpp>
#include <unordered_map>

namespace Pika
{
	enum class ShaderDataType
	{
		None = 0, Float, Float2, Float3, Float4, Int, Int2, Int3, Int4, Mat3, Mat4, Bool
	};

	struct ShaderSourceMarkers
	{
		std::string VertexShaderBegin = R"(#VERTEX_BEGIN())";
		std::string VertexShaderEnd = R"(#VERTEX_END())";
		std::string GeometryShaderBegin = R"(#GEOMETRY_BEGIN())";
		std::string GeometryShaderEnd = R"(#GEOMETRY_END())";
		std::string FragmentShaderBegin = R"(#FRAGMENT_BEGIN())";
		std::string FragmentShaderEnd = R"(#FRAGMENT_END())";
		ShaderSourceMarkers() = default;
	};

	class Shader
	{
	public:
		virtual ~Shader() = default;
		virtual void bind() const = 0;
		virtual void unbind() const = 0;
		virtual const std::string& getName() const = 0;
		virtual void setName(const std::string& vName) = 0;

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
		virtual void setIntArray(const std::string& vName, const int* vValue, uint32_t vCount) const = 0;

		static Ref<Shader> Create(const std::filesystem::path& vFilePath);
		static Ref<Shader> Create(const std::string& vName, const std::string& vFilePath);
		static Ref<Shader> Create(const std::string& vName, const std::string& vVertexShaderSrc, const std::string& vFragmentShaderSrc);
	public:
		inline bool isInShaderLibrary() const { return m_InShaderLibrary; }
		bool m_InShaderLibrary = false; //All shaders must be loaded into the shader library 
										//before they can be used.
	protected:
		static ShaderSourceMarkers s_FileMarkers;
	};

	class ShaderLibrary
	{
	public:
		void add(const Ref<Shader>& vShader);
		void add(const std::string& vName, const Ref<Shader>& vShader);
		Ref<Shader> load(const std::string& vFilePath);
		Ref<Shader> load(const std::string& vName, const std::string& vFilePath);
		void remove(const std::string& vName);

		Ref<Shader> getShader(const std::string& vName);
	private:
		bool isExist(const std::string& vName) const;
	private:
		std::unordered_map<std::string, Ref<Shader>> m_Shaders;
	};

}

