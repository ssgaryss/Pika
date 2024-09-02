#pragma once
#include <cstdint>

namespace Pika {

	template <typename T>
	class RenderBatch
	{
	public:
		RenderBatch() = default;
		RenderBatch(uint32_t vSize);

		uint32_t getBatchSize() const;

		T& operator[](uint32_t vIndex);
		const T& operator[](uint32_t vIndex) const;
	private:
		std::vector<T> m_Buffer;
		uint32_t m_BufferIndex = 0;    // 最后一个有效实例的Index
	};

	template<typename T>
	inline RenderBatch<T>::RenderBatch(uint32_t vSize)
		: m_Buffer{ vSize }
	{
	}

	template<typename T>
	inline uint32_t RenderBatch<T>::getBatchSize() const
	{
		return static_cast<uint32_t>(m_Buffer.size());
	}

	template<typename T>
	inline T& RenderBatch<T>::operator[](uint32_t vIndex)
	{
		return m_Buffer[vIndex];
	}

	template<typename T>
	inline const T& RenderBatch<T>::operator[](uint32_t vIndex) const
	{
		return m_Buffer[vIndex];
	}

}
