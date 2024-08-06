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

	void SceneRenderer::beginFrame()
	{
		m_Framebuffer->bind();
	}

	void SceneRenderer::endFrame()
	{
		m_Framebuffer->unbind();
	}

	void SceneRenderer::render()
	{
		// TODO
		auto View = m_Scene->m_Registry.group<TransformComponent, SpriteRendererComponent>();
		for (auto& Entity : View) {
			auto [Transform, SpriteRenderer] = View.get<TransformComponent, SpriteRendererComponent>(Entity); // 此处得到的是tuple，C++17开始对tuple的结构化绑定可以自动推导引用
			Renderer2D::DrawSprite(Transform, SpriteRenderer, static_cast<int>(Entity));
		}
	}

	void SceneRenderer::render(const EditorCamera& vEditorCamera)
	{
		Renderer2D::BeginScene(vEditorCamera);  // TODO : Renderer3D
		auto View = m_Scene->m_Registry.group<TransformComponent, SpriteRendererComponent>();
		for (auto& Entity : View) {
			auto [Transform, SpriteRenderer] = View.get<TransformComponent, SpriteRendererComponent>(Entity); // 此处得到的是tuple，C++17开始对tuple的结构化绑定可以自动推导引用
			Renderer2D::DrawSprite(Transform, SpriteRenderer, static_cast<int>(Entity));
		}
		Renderer2D::EndScene();
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