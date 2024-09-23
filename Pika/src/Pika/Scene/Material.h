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
		inline std::string getMaterialType() const override { return m_MaterialType; }

		inline const glm::vec3& getAmbient() const { return m_Ambient; }
		inline glm::vec3& getAmbient() { return m_Ambient; }
		inline void setAmbient(const glm::vec3& vAmbient) { m_Ambient = vAmbient; }
		inline const glm::vec3& getDiffuse() const { return m_Diffuse; }
		inline glm::vec3& getDiffuse() { return m_Diffuse; }
		inline void setDiffuse(const glm::vec3& vDiffuse) { m_Diffuse = vDiffuse; }
		inline const glm::vec3& getSpecular() const { return m_Specular; }
		inline glm::vec3& getSpecular() { return m_Specular; }
		inline void setSpecular(const glm::vec3& vSpecular) { m_Specular = vSpecular; }
	private:
		inline static const std::string m_MaterialType = "Blinn-Phone";
		glm::vec3 m_Ambient;
		glm::vec3 m_Diffuse;
		glm::vec3 m_Specular;
	};

}
