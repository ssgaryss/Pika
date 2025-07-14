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
			float m_Shininess = 5.0f;
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

	class StandardPBRMaterial : public Material {
	public:
		struct Data {
			glm::vec3 m_Albedo = glm::vec3(1.0f);
			float m_Metallic = 0.0f;
			float m_Roughness = 0.5f;
			float m_AmbientOcclusion = 1.0f;

			Ref<Texture2D> m_AlbedoMap = nullptr;
			Ref<Texture2D> m_MetallicMap = nullptr;
			Ref<Texture2D> m_RoughnessMap = nullptr;
			Ref<Texture2D> m_AmbientOcclusionMap = nullptr;
			Ref<Texture2D> m_NormalMap = nullptr;
		};
	public:
		StandardPBRMaterial() = default;
		StandardPBRMaterial(const Data& vData)
			: m_Data{ vData } {}
		StandardPBRMaterial(const StandardPBRMaterial&) = default;
		StandardPBRMaterial& operator=(const StandardPBRMaterial&) = default;
		StandardPBRMaterial(StandardPBRMaterial&&) = default;
		StandardPBRMaterial& operator=(StandardPBRMaterial&&) noexcept = default;
		inline const std::string& getType() const override { return s_Type; }
		inline Ref<Material> clone() const override { return CreateRef<StandardPBRMaterial>(*this); }

		inline const Data& getData() const { return m_Data; }
		inline Data& getData() { return m_Data; }
		inline void setData(const Data& vData) { m_Data = vData; }
	private:
		inline static const std::string s_Type = "Standard PBR";
		Data m_Data;
	};

}
