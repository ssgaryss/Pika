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
		if (!m_PrimaryCamera)
			return;
		switch (m_Context->getSceneType())
		{
		case Scene::SceneType::Scene2D:
		{
			const auto& TC = m_PrimaryCamera.getComponent<TransformComponent>();
			const auto& ViewMatrix = glm::inverse(glm::translate(glm::mat4(1.0f), TC.m_Position) *
				glm::toMat4(glm::quat(glm::radians(TC.m_Rotation))));
			Renderer2D::BeginScene(m_PrimaryCamera.getComponent<CameraComponent>().m_Camera, ViewMatrix);
			auto View = m_Context->m_Registry.group<TransformComponent, SpriteRendererComponent>();
			for (const auto& Entt : View) {
				auto [Transform, SpriteRenderer] = View.get<TransformComponent, SpriteRendererComponent>(Entt); // 此处得到的是tuple，C++17开始对tuple的结构化绑定可以自动推导引用
				Renderer2D::DrawSprite(Transform, SpriteRenderer, static_cast<int>(Entt));
			}
			Renderer2D::EndScene();
			return;
		}
		case Scene::SceneType::Scene3D:
		{
			// TODO
			//PK_CORE_ERROR("No 3d play mode yet");
			return;
		}
		}
	}

	void SceneRenderer::render(const EditorCamera& vEditorCamera)
	{
		switch (m_Context->getSceneType())
		{
		case Scene::SceneType::Scene2D:
		{
			Renderer2D::BeginScene(vEditorCamera);
			if (m_Settings.m_ShowGrid)
				Renderer2D::DrawGrid(glm::mat4(1.0f), 100.5f);
			auto View = m_Context->m_Registry.group<TransformComponent, SpriteRendererComponent>();
			for (const auto& Entt : View) {
				auto [Transform, SpriteRenderer] = View.get<TransformComponent, SpriteRendererComponent>(Entt); // 此处得到的是tuple，C++17开始对tuple的结构化绑定可以自动推导引用
				Renderer2D::DrawSprite(Transform, SpriteRenderer, static_cast<int>(Entt));
			}
			Renderer2D::EndScene();
			return;
		}
		case Scene::SceneType::Scene3D:
		{
			Renderer3D::BeginScene(vEditorCamera);
			if (m_Settings.m_ShowGrid)
				Renderer3D::DrawGrid(glm::mat4(1.0f), 100.5f);
			// TODO !
			Renderer3D::EndScene();
			return;
			return;
		}
		}
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