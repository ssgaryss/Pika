#include "pkpch.h"
#include "SceneRenderer.h"
#include "Pika/Renderer/Renderer2D.h"
#include "Pika/Renderer/Renderer3D.h"

namespace Pika {

	SceneRenderer::SceneRenderer(const Ref<Scene>& vScene, const Ref<Framebuffer>& vFramebuffer)
		: m_Context{ vScene }, m_Framebuffer{ vFramebuffer }
	{
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
		auto View = m_Context->m_Registry.group<TransformComponent, SpriteRendererComponent>();
		for (auto& Entity : View) {
			auto [Transform, SpriteRenderer] = View.get<TransformComponent, SpriteRendererComponent>(Entity); // �˴��õ�����tuple��C++17��ʼ��tuple�Ľṹ���󶨿����Զ��Ƶ�����
			Renderer2D::DrawSprite(Transform, SpriteRenderer, static_cast<int>(Entity));
		}
	}

	void SceneRenderer::render(const EditorCamera& vEditorCamera)
	{
		Renderer2D::BeginScene(vEditorCamera);  // TODO : Renderer3D
		auto View = m_Context->m_Registry.group<TransformComponent, SpriteRendererComponent>();
		for (auto& Entity : View) {
			auto [Transform, SpriteRenderer] = View.get<TransformComponent, SpriteRendererComponent>(Entity); // �˴��õ�����tuple��C++17��ʼ��tuple�Ľṹ���󶨿����Զ��Ƶ�����
			Renderer2D::DrawSprite(Transform, SpriteRenderer, static_cast<int>(Entity));
		}
		Renderer2D::EndScene();
	}

	inline std::vector<std::string> SceneRenderer::getAllCameras() const
	{
		auto View = m_Context->m_Registry.group<CameraComponent, TransformComponent>();
		for (auto& Entity : View) {
			auto [Camera, Transform] = View.get<CameraComponent, TransformComponent>(Entity);

		}
		return std::vector<std::string>();
	}

	void SceneRenderer::resize(uint32_t vWidth, uint32_t vHeight)
	{
		m_Framebuffer->resize(vWidth, vHeight);
	}

	void SceneRenderer::initialize()
	{
		switch (m_Context->getSceneType())
		{
		case Pika::Scene::SceneType::Scene2D:
		{
			Renderer2D::Initialize();
			break;
		}
		case Pika::Scene::SceneType::Scene3D:
		{
			Renderer3D::Initialize();
			break;
		}
		}
	}

}