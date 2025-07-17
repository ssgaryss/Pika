#include "pkpch.h"
#include "SceneRenderer.h"
#include "Pika/Renderer/RenderCommand.h"
#include "Pika/Renderer/Renderer2D.h"
#include "Pika/Renderer/Renderer3D.h"
#include "Pika/Renderer/Bakers.h"

namespace Pika {


	SceneRenderer::SceneRenderer(const Ref<Scene>& vScene, const Ref<Framebuffer>& vFramebuffer)
		: m_Framebuffer{ vFramebuffer }
	{
		m_RenderDataExtractor = CreateRef<RenderDataExtractor>(vScene);
	}

	void SceneRenderer::beginFrame()
	{
		m_Framebuffer->bind();
		RenderCommand::SetClearColor({ 0.2f, 0.2f, 0.2f, 1.0f });
		RenderCommand::Clear(); // 会影响所有FBO中的Texture而非一个
		m_Framebuffer->clearAttachment(1, -1); // 所有EntityID其余区域赋值-1

	}

	void SceneRenderer::endFrame()
	{
		m_Framebuffer->unbind();
	}

	void SceneRenderer::render()
	{
		if (!m_PrimaryCamera)
			return;
		switch (m_RenderDataExtractor->extractSceneType())
		{
		case Scene::SceneType::Scene2D:
		{
			const auto& TC = m_PrimaryCamera.getComponent<TransformComponent>();
			const auto& ViewMatrix = glm::inverse(glm::translate(glm::mat4(1.0f), TC.m_Position) *
				glm::toMat4(glm::quat(glm::radians(TC.m_Rotation))));
			beginFrame();
			Renderer2D::BeginScene(m_PrimaryCamera.getComponent<CameraComponent>().m_Camera, ViewMatrix);
			auto SpritesData = m_RenderDataExtractor->extractComponentsWithEntityID<TransformComponent, SpriteRendererComponent>();
			for (const auto& SpriteData : SpritesData) {
				auto [Transform, SpriteRenderer, EntityID] = SpriteData;
				Renderer2D::DrawSprite(Transform, SpriteRenderer, EntityID);
			}
			Renderer2D::EndScene();
			endFrame();
			return;
		}
		case Scene::SceneType::Scene3D:
		{
			auto LightsData = m_RenderDataExtractor->extractLightsData();
			auto SceneData = m_RenderDataExtractor->extractSceneData();
			Renderer3D::DrawShadowMaps(LightsData, SceneData); // Shadow Map
			beginFrame();
			const auto& TC = m_PrimaryCamera.getComponent<TransformComponent>();
			const auto& ViewMatrix = glm::inverse(glm::translate(glm::mat4(1.0f), TC.m_Position) *
				glm::toMat4(glm::quat(glm::radians(TC.m_Rotation))));
			beginFrame();
			Renderer3D::BeginScene(m_PrimaryCamera.getComponent<CameraComponent>().m_Camera, ViewMatrix, LightsData);
			auto NoMaterialModelsData = m_RenderDataExtractor->extractNoMaterialModelsWithEntityID();
			for (const auto& NoMaterialModelData : NoMaterialModelsData) {
				auto [Transform, Model, EntityID] = NoMaterialModelData;
				Renderer3D::DrawModel(Transform, Model, EntityID);
			}
			auto BlinnPhoneMaterialModelsData = m_RenderDataExtractor->extractBlinnPhoneMaterialModelsWithEntityID();
			for (const auto& BlinnPhoneMaterialModelData : BlinnPhoneMaterialModelsData) {
				auto [Transform, Model, Material, EntityID] = BlinnPhoneMaterialModelData;
				Renderer3D::DrawModel(Transform, Model, Material, EntityID);
			}
			Renderer3D::EndScene();
			endFrame();
			return;
		}
		}
	}

	void SceneRenderer::render(const EditorCamera& vEditorCamera)
	{
		switch (m_RenderDataExtractor->extractSceneType())
		{
		case Scene::SceneType::Scene2D:
		{
			beginFrame();
			Renderer2D::BeginScene(vEditorCamera);
			if (m_Settings.m_ShowGrid)
				Renderer2D::DrawGrid(glm::mat4(1.0f), 100.5f);
			auto SpritesData = m_RenderDataExtractor->extractComponentsWithEntityID<TransformComponent, SpriteRendererComponent>();
			for (const auto& SpriteData : SpritesData) {
				auto [Transform, SpriteRenderer, EntityID] = SpriteData;
				Renderer2D::DrawSprite(Transform, SpriteRenderer, EntityID);
			}
			Renderer2D::EndScene();
			endFrame();
			return;
		}
		case Scene::SceneType::Scene3D:
		{
			auto LightsData = m_RenderDataExtractor->extractLightsData();
			auto SceneData = m_RenderDataExtractor->extractSceneData();
			Renderer3D::DrawShadowMaps(LightsData, SceneData); // Shadow Map
			beginFrame();
			Renderer3D::BeginScene(vEditorCamera, LightsData);
			Renderer3D::DrawSkybox(m_RenderDataExtractor->extractSkybox());
			if (m_Settings.m_ShowGrid)
				Renderer3D::DrawGrid(glm::mat4(1.0f), 100.5f);
			auto NoMaterialModelsData = m_RenderDataExtractor->extractNoMaterialModelsWithEntityID();
			for (const auto& NoMaterialModelData : NoMaterialModelsData) {
				auto [Transform, Model, EntityID] = NoMaterialModelData;
				Renderer3D::DrawModel(Transform, Model, EntityID);
			}
			auto BlinnPhoneMaterialModelsData = m_RenderDataExtractor->extractBlinnPhoneMaterialModelsWithEntityID();
			for (const auto& BlinnPhoneMaterialModelData : BlinnPhoneMaterialModelsData) {
				auto [Transform, Model, Material, EntityID] = BlinnPhoneMaterialModelData;
				Renderer3D::DrawModel(Transform, Model, Material, EntityID);
			}
			Renderer3D::EndScene();
			endFrame();
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
		PK_CORE_TRACE("SceneRenderer : Try to initialize ...");
		IBLBaker::Initialize();

		switch (m_RenderDataExtractor->extractSceneType())
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
