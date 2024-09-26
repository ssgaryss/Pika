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
			float m_Intensity = 1.0f;                               // ��Դǿ��
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


}
