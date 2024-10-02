#include "pkpch.h"
#include "ShadowRenderer.h"

namespace Pika {

	ShadowRenderer::ShadowRenderer()
	{
		m_ShadowBuffer = Framebuffer::Create({ m_Width, m_Height, 1,
			{ TextureFormat::DEPTH24STENCIL8 }, false });
	}

}