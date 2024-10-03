#pragma once
#include <cstdint>

namespace Pika {

	template <typename T>
	class RenderBatch
	{
	public:
		RenderBatch() = default;
		RenderBatch(uint32_t vSize);

		uint32_t getRenderBatchSize() const;

		bool isFull() const;
		bool empty() const;
		void add(const T& vElement);
		void reset();
		T* data();
		uint32_t size() const;     // 返回已有数据byte size

		const std::vector<uint32_t>& getIndices() const;

		T& operator[](uint32_t vIndex);
		const T& operator[](uint32_t vIndex) const;
	private:
		uint32_t m_RenderBatchSize = 1000;
		std::vector<T> m_Buffer;
		uint32_t m_BufferIndex = 0;        // 最后一个有效实例的Index
		std::vector<uint32_t> m_Indices;
		uint32_t m_IndexCount = 0;
	};

	template<typename T>
	inline RenderBatch<T>::RenderBatch(uint32_t vSize)
		: m_RenderBatchSize{ vSize }
	{
		m_Buffer.resize(m_RenderBatchSize);
		m_Indices.resize(m_RenderBatchSize);
		std::iota(m_Indices.begin(), m_Indices.end(), 0);
	}

	template<typename T>
	inline uint32_t RenderBatch<T>::getRenderBatchSize() const
	{
		return m_RenderBatchSize;
	}

	template<typename T>
	inline bool RenderBatch<T>::isFull() const
	{
		return m_BufferIndex == m_RenderBatchSize;
	}

	template<typename T>
	inline bool RenderBatch<T>::empty() const
	{
		return m_BufferIndex == 0;
	}

	template<typename T>
	inline void RenderBatch<T>::add(const T& vElement)
	{
		if (m_BufferIndex > m_RenderBatchSize)
			throw std::runtime_error("RenderBatch : Batch is full, flush it first !");
		m_Buffer[m_BufferIndex] = vElement;
		m_BufferIndex++;
	}

	template<typename T>
	inline void RenderBatch<T>::reset()
	{
		m_BufferIndex = 0;
	}

	template<typename T>
	inline T* RenderBatch<T>::data()
	{
		return m_Buffer.data();
	}

	template<typename T>
	inline uint32_t RenderBatch<T>::size() const
	{
		return m_BufferIndex * sizeof(T);
	}

	template<typename T>
	inline const std::vector<uint32_t>& RenderBatch<T>::getIndices() const
	{
		return m_Indices;
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
