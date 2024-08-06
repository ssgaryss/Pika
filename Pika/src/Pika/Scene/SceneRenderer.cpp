#include "pkpch.h"
#include "SceneRenderer.h"
#include "Pika/Renderer/Renderer2D.h"
#include "Pika/Renderer/Renderer3D.h"

namespace Pika {

	SceneRenderer::SceneRenderer(const Ref<Scene>& vScene, const Ref<Framebuffer>& vFramebuffer)
		: m_Scene{ vScene }, m_Framebuffer{ vFramebuffer }
	{
		initialize();
	}

	void SceneRenderer::beginRender(const Camera& vCamera)
	{

	}

	void SceneRenderer::endRender()
	{
	}

	void SceneRenderer::resize(uint32_t vWidth, uint32_t vHeight)
	{
		m_Framebuffer->resize(vWidth, vHeight);
	}

	void SceneRenderer::initialize()
	{
		switch (m_RendererType)
		{
		case Pika::SceneRenderer::Renderer2D:
		{
			Renderer2D::Initialize();
			break;
		}
		case Pika::SceneRenderer::Renderer3D:
		{
			Renderer3D::Initialize();
			break;
		}
		}
	}

}