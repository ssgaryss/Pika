#pragma once
#include "Pika/Renderer/Framebuffer.h"
#include "Pika/Renderer/Texture.h"

namespace Pika {

	class ShadowRenderer
	{
	public:
		ShadowRenderer();

		Ref<Texture2D> renderDirectionLightShadowMap();
	private:
		uint32_t m_Width = 0;
		uint32_t m_Height = 0;
		Ref<Framebuffer> m_ShadowBuffer = nullptr;
	};

}
