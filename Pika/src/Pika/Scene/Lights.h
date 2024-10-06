#pragma once
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

namespace Pika {


	class Light
	{
	public:
		virtual ~Light() = default;

		virtual const std::string& getType() const = 0;
		virtual Ref<Light> clone() const = 0;
	};

	class DirectionLight : public Light
	{
	public:
		struct Data
		{
			glm::vec3 m_LightColor = glm::vec3(1.0f);               // 光源颜色
			float m_Intensity = 1.0f;                               // 强度
			bool m_EnableShadow = false;
			Ref<Texture2D> m_ShadowMap = nullptr;
			float m_LightRegionSize = 10.0f;                        // 计算LightProjectionMatrix所需参数

			inline static const glm::vec3 s_DefaultDirection = glm::vec3(0.0f, 0.0f, -1.0f); // 默认照向-z方向
			glm::mat4 getLightProjectionMatrix() const {
				return glm::ortho(-m_LightRegionSize, m_LightRegionSize, -m_LightRegionSize, m_LightRegionSize, -m_LightRegionSize, m_LightRegionSize);
			}
		};
	public:
		DirectionLight() = default;  // 由于基类申明了析构函数，这里不能偷懒，满足rule of five，因为Component之后可能会多次拷贝等
		DirectionLight(const Data& vData)
			:m_Data{ vData } {}
		DirectionLight(const DirectionLight&) = default;
		DirectionLight& operator=(const DirectionLight&) = default;
		DirectionLight(DirectionLight&&) noexcept = default;
		DirectionLight& operator=(DirectionLight&&) noexcept = default;
		~DirectionLight() = default;

		inline const std::string& getType() const override { return m_Type; }
		inline Ref<Light> clone() const override { return CreateRef<DirectionLight>(*this); }

		inline const Data& getData() const { return m_Data; }
		inline Data& getData() { return m_Data; }
		inline void setData(const Data& vData) { m_Data = vData; }
	private:
		inline static const std::string m_Type = "Direction Light";
		Data m_Data;
	};

	class PointLight : public Light
	{
	public:
		struct Data
		{
			glm::vec3 m_LightColor = glm::vec3(1.0f);               // 光源颜色
			float m_Intensity = 1.0f;                               // 光源强度
			float m_Constant = 1.0f;                                // 常数衰减项
			float m_Linear = 0.07f;                                 // 线性衰减项
			float m_Quadratic = 0.017f;                             // 二次衰减项
			// I = m_Intensity / ( m_Constant + m_Linear * distance + m_Quadratic * distance * distance )

			bool m_EnableShadow = false;
			Ref<Cubemap> m_ShadowMap = nullptr;
		};
	public:
		PointLight() = default;  // 由于基类申明了析构函数，这里不能偷懒，满足rule of five，因为Component之后可能会多次拷贝等
		PointLight(const Data& vData)
			:m_Data{ vData } {}
		PointLight(const PointLight&) = default;
		PointLight& operator=(const PointLight&) = default;
		PointLight(PointLight&&) noexcept = default;
		PointLight& operator=(PointLight&&) noexcept = default;
		~PointLight() = default;

		inline const std::string& getType() const override { return m_Type; }
		inline Ref<Light> clone() const override { return CreateRef<PointLight>(*this); }

		inline const Data& getData() const { return m_Data; }
		inline Data& getData() { return m_Data; }
		inline void setData(const Data& vData) { m_Data = vData; }
	private:
		inline static const std::string m_Type = "Point Light";
		Data m_Data;
	};

	class SpotLight : public Light
	{
	public:
		// TODO ! 
	private:
	};


}
