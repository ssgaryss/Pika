#pragma once
#include "Pika/Renderer/UniformBuffer.h"

namespace Pika {

	class OpenGLUniformBuffer : public UniformBuffer
	{
	public:
		OpenGLUniformBuffer(uint32_t vSize, uint32_t vBinding);
		~OpenGLUniformBuffer() = default;

		void setData(const void* vData, uint32_t vSize, uint32_t vOffset = 0) override;
	private:
		uint32_t m_RendererID = 0;
	};

}
