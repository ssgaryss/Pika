#include "pkpch.h"
#include "ShadowRenderer.h"

namespace Pika {


	ShadowRenderer::ShadowRenderer(const Ref<Scene>& vScene, const Ref<Framebuffer>& vFramebuffer)
		: m_ShadowBuffer{ vFramebuffer }
	{
	}

}