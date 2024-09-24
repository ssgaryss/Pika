#pragma once
#include <glm/glm.hpp>

namespace Pika {

	class Material
	{
	public:
		virtual ~Material() = default;

		virtual std::string getType() const = 0;
		virtual Ref<Material> clone() const = 0;
	};

	class BlinnPhoneMaterial : public Material
	{
	public:
		struct Data {
			glm::vec3 m_Ambient;
			glm::vec3 m_Diffuse;
			glm::vec3 m_Specular;
			Data() = default;
		};
	public:
		BlinnPhoneMaterial() = default;
		BlinnPhoneMaterial(const BlinnPhoneMaterial&) = default;
		BlinnPhoneMaterial& operator=(const BlinnPhoneMaterial&) = default;
		BlinnPhoneMaterial(BlinnPhoneMaterial&&) = default;
		BlinnPhoneMaterial& operator=(BlinnPhoneMaterial&&) noexcept = default;
		inline std::string getType() const override { return m_MaterialType; }
		inline Ref<Material> clone() const override { return CreateRef<BlinnPhoneMaterial>(*this); } // 原型模式！

		inline const Data& getData() const { return m_Data; }
		inline Data& getData() { return m_Data; }
		inline void setData(const Data& vData) { m_Data = vData; }
	private:
		inline static const std::string m_MaterialType = "Blinn-Phone";
		Data m_Data;
	};

}
