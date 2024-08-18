#pragma once

namespace Pika {

	class UniformBuffer
	{
	public:
		virtual ~UniformBuffer() = default;
		virtual void setData(const void* vData, uint32_t vSize, uint32_t vOffset = 0) = 0;
		static Ref<UniformBuffer> Create(uint32_t vSize, uint32_t vBinding);
	};

}
