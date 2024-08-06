#include "pkpch.h"
#include "SceneRenderer.h"
#include "Pika/Renderer/Renderer2D.h"
#include "Pika/Renderer/Renderer3D.h"

namespace Pika {
	SceneRenderer::SceneRenderer(const Ref<Scene>& vScene)
	{
	}
	void SceneRenderer::render(const Camera& vCamera)
	{
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