#pragma once
#include "Pika/Renderer/Texture.h"
#include <glm/glm.hpp>

namespace Pika {

	class Material
	{
	public:
		virtual ~Material() = default;

		virtual const std::string& getType() const = 0;
		virtual Ref<Material> clone() const = 0;
	};

	class BlinnPhoneMaterial : public Material
	{
	public:
		struct Data {
			glm::vec3 m_Ambient = glm::vec3(1.0f);
			glm::vec3 m_Diffuse = glm::vec3(1.0f);
			glm::vec3 m_Specular = glm::vec3(1.0f);
			float m_Shininess = 0.0f;
			Ref<Texture2D> m_DiffuseMap = nullptr;
			Ref<Texture2D> m_SpecularMap = nullptr;
		};
	public:
		BlinnPhoneMaterial() = default;
		BlinnPhoneMaterial(const Data& vData)
			: m_Data{ vData } {}
		BlinnPhoneMaterial(const BlinnPhoneMaterial&) = default;
		BlinnPhoneMaterial& operator=(const BlinnPhoneMaterial&) = default;
		BlinnPhoneMaterial(BlinnPhoneMaterial&&) = default;
		BlinnPhoneMaterial& operator=(BlinnPhoneMaterial&&) noexcept = default;
		inline const std::string& getType() const override { return s_Type; }
		inline Ref<Material> clone() const override { return CreateRef<BlinnPhoneMaterial>(*this); } // 原型模式！

		inline const Data& getData() const { return m_Data; }
		inline Data& getData() { return m_Data; }
		inline void setData(const Data& vData) { m_Data = vData; }
	private:
		inline static const std::string s_Type = "Blinn-Phone";
		Data m_Data;
	};

}
