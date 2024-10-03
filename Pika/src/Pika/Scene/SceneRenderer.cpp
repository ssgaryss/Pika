#include "pkpch.h"
#include "SceneRenderer.h"
#include "Pika/Renderer/Renderer2D.h"
#include "Pika/Renderer/Renderer3D.h"

namespace Pika {


	SceneRenderer::SceneRenderer()
	{

	}

	SceneRenderer::SceneRenderer(const Ref<Scene>& vScene, const Ref<Framebuffer>& vFramebuffer)
		: m_Framebuffer{ vFramebuffer }
	{
		m_RenderDataExtracor = CreateRef<RenderDataExtractor>(vScene);
	}

	void SceneRenderer::beginFrame()
	{
		m_Framebuffer->bind();
	}

	void SceneRenderer::endFrame()
	{
		m_Framebuffer->unbind();
	}

	void SceneRenderer::render	()
	{
		if (!m_PrimaryCamera)
			return;
		switch (m_RenderDataExtracor->extractSceneType())
		{
		case Scene::SceneType::Scene2D:
		{
			const auto& TC = m_PrimaryCamera.getComponent<TransformComponent>();
			const auto& ViewMatrix = glm::inverse(glm::translate(glm::mat4(1.0f), TC.m_Position) *
				glm::toMat4(glm::quat(glm::radians(TC.m_Rotation))));
			Renderer2D::BeginScene(m_PrimaryCamera.getComponent<CameraComponent>().m_Camera, ViewMatrix);
			auto SpritesData = m_RenderDataExtracor->extractComponentsWithEntityID<TransformComponent, SpriteRendererComponent>();
			for (const auto& SpriteData : SpritesData) {
				auto [Transform, SpriteRenderer, EntityID] = SpriteData;
				Renderer2D::DrawSprite(Transform, SpriteRenderer, EntityID);
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
		switch (m_RenderDataExtracor->extractSceneType())
		{
		case Scene::SceneType::Scene2D:
		{
			Renderer2D::BeginScene(vEditorCamera);
			if (m_Settings.m_ShowGrid)
				Renderer2D::DrawGrid(glm::mat4(1.0f), 100.5f);
			auto SpritesData = m_RenderDataExtracor->extractComponentsWithEntityID<TransformComponent, SpriteRendererComponent>();
			for (const auto& SpriteData : SpritesData) {
				auto [Transform, SpriteRenderer, EntityID] = SpriteData;
				Renderer2D::DrawSprite(Transform, SpriteRenderer, EntityID);
			}
			Renderer2D::EndScene();
			return;
		}
		case Scene::SceneType::Scene3D:
		{
			auto LightsData = m_RenderDataExtracor->extractLightsData();
			Renderer3D::BeginScene(vEditorCamera, LightsData);
			Renderer3D::RenderSkybox(m_RenderDataExtracor->extractSkybox());
			if (m_Settings.m_ShowGrid)
				Renderer3D::DrawGrid(glm::mat4(1.0f), 100.5f);
			auto NoMaterialModelsData = m_RenderDataExtracor->extractNoMaterialModelsWithEntityID();
			for (const auto& NoMaterialModelData : NoMaterialModelsData) {
				auto [Transform, Model, EntityID] = NoMaterialModelData;
				Renderer3D::DrawModel(Transform, Model, EntityID);
			}
			auto BlinnPhoneMaterialModelsData = m_RenderDataExtracor->extractBlinnPhoneMaterialModelsWithEntityID();
			for (const auto& BlinnPhoneMaterialModelData : BlinnPhoneMaterialModelsData) {
				auto [Transform, Model, Material, EntityID] = BlinnPhoneMaterialModelData;
				Renderer3D::DrawModel(Transform, Model, Material, EntityID);
			}

			Renderer3D::EndScene();
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
		switch (m_RenderDataExtracor->extractSceneType())
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