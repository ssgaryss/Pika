#pragma once
#include <glm/glm.hpp>

namespace Pika {


	class Light
	{
	public:
		virtual ~Light() = default;

		virtual std::string getType() const = 0;
		virtual Ref<Light> clone() const = 0;
	};

	class DirectionLight : public Light
	{
	public:
	private:
	};

	class PointLight : public Light
	{
	public:
		struct Data
		{
			glm::vec3 m_LightColor = glm::vec3(1.0f);               // ��Դ��ɫ
			float m_Intensity = 0.0f;                               // ��Դǿ��
			float m_Constant = 1.0f;                                // ����˥����
			float m_Linear = 0.07f;                                 // ����˥����
			float m_Quadratic = 0.017f;                             // ����˥����
			Data() = default;
			// I = m_Intensity / ( m_Constant + m_Linear * distance + m_Quadratic * distance * distance )
		};
	public:
		PointLight() = default;  // ���ڻ����������������������ﲻ��͵��������rule of five����ΪComponent֮����ܻ��ο�����
		PointLight(const PointLight&) = default;
		PointLight& operator=(const PointLight&) = default;
		PointLight(PointLight&&) noexcept = default;
		PointLight& operator=(PointLight&&) noexcept = default;
		~PointLight() = default;

		inline std::string getType() const override { return m_Type; }
		inline Ref<Light> clone() const override { return CreateRef<PointLight>(*this); }

		inline const Data& getData() const { return m_Data; }
		inline Data& getData() { return m_Data; }
		inline void setData(const Data& vData) { m_Data = vData; }
	private:
		inline static const std::string m_Type = "Point Light";
		Data m_Data;
	};

	struct LightsData
	{
		struct UniformBufferSTD140PointLightData
		{
			// �����ڴ������Ϊ������std140��vec3���뵽16�ֽ�
			alignas(16) glm::vec3 m_Position;                       // ��Դλ��
			alignas(16) glm::vec3 m_LightColor = glm::vec3(1.0f);   // ��Դ��ɫ
			float m_Intensity = 0.0f;                               // ��Դǿ��
			float m_Constant = 1.0f;                                // ����˥����
			float m_Linear = 0.07f;                                 // ����˥����
			float m_Quadratic = 0.017f;                             // ����˥����
			UniformBufferSTD140PointLightData() = default;
			UniformBufferSTD140PointLightData(const glm::vec3& vPosition, const PointLight::Data& vPointLightData)
				: m_Position{ vPosition }, m_LightColor{ vPointLightData.m_LightColor },
				m_Intensity{ vPointLightData.m_Intensity }, m_Constant{ vPointLightData.m_Constant },
				m_Linear{ vPointLightData.m_Linear }, m_Quadratic{ vPointLightData.m_Quadratic } {}
			void setData(const glm::vec3& vPosition, const PointLight::Data& vPointLightData) {
				m_Position = vPosition;
				m_LightColor = vPointLightData.m_LightColor;
				m_Intensity = vPointLightData.m_Intensity;
				m_Constant = vPointLightData.m_Constant;
				m_Linear = vPointLightData.m_Linear;
				m_Quadratic = vPointLightData.m_Quadratic;
			}
		};
		//std::array<UniformBufferSTD140PointLightData, 4> m_PointLightsData;
		UniformBufferSTD140PointLightData m_PointLightsData; // TODO : Delete��
	};


}
