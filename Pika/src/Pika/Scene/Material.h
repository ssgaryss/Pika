#pragma once
#include <glm/glm.hpp>

namespace Pika {

	class Material
	{
	public:
		virtual ~Material() = default;

		virtual std::string getMaterialType() const = 0;
	};

	class BlinnPhoneMaterial : public Material
	{
	public:
		struct Data {
			glm::vec3 m_Ambient;
			glm::vec3 m_Diffuse;
			glm::vec3 m_Specular;
		};
	public:
		inline std::string getMaterialType() const override { return m_MaterialType; }

		inline const Data& getData() const { return m_Data; }
		inline Data& getData() { return m_Data; }
		inline void setData(const Data& vData) { m_Data = vData; }
	private:
		inline static const std::string m_MaterialType = "Blinn-Phone";
		Data m_Data;
	};

}
