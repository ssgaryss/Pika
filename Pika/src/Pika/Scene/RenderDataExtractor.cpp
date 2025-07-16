#include "pkpch.h"
#include "RenderDataExtractor.h"
#include "Components.h"
#include "Lights.h"
#include "Materials.h"

namespace Pika {


	Scene::SceneType RenderDataExtractor::extractSceneType() const
	{
		return m_Scene->getSceneType();
	}

	const Ref<Cubemap>& RenderDataExtractor::extractSkybox() const
	{
		return m_Scene->m_Environment->getSkybox();
	}

	std::vector<std::tuple<TransformComponent&, ModelComponent&, int>> RenderDataExtractor::extractNoMaterialModelsWithEntityID() const
	{
		auto View = m_Scene->m_Registry.view<TransformComponent, ModelComponent>();
		std::vector<std::tuple<TransformComponent&, ModelComponent&, int>> EntityData;
		for (const auto& Entt : View) {
			if (!m_Scene->m_Registry.any_of<MaterialComponent>(Entt))
				EntityData.emplace_back(std::tuple_cat(View.get<TransformComponent, ModelComponent>(Entt),
					std::make_tuple(static_cast<int>(Entt))));
		}
		return EntityData;
	}

	std::vector<std::tuple<TransformComponent&, ModelComponent&, MaterialComponent&, int>> RenderDataExtractor::extractBlinnPhoneMaterialModelsWithEntityID() const
	{
		auto View = m_Scene->m_Registry.view<TransformComponent, ModelComponent, MaterialComponent>();
		std::vector<std::tuple<TransformComponent&, ModelComponent&, MaterialComponent&, int>> EntityData;
		for (const auto& Entt : View) {
			auto& Material = View.get<MaterialComponent>(Entt);
			if (dynamic_cast<BlinnPhoneMaterial*>(Material.m_Material.get()))
				EntityData.emplace_back(std::tuple_cat(View.get<TransformComponent, ModelComponent, MaterialComponent>(Entt),
					std::make_tuple(static_cast<int>(Entt))));
		}
		return EntityData;
	}

	std::vector<std::tuple<TransformComponent&, LightComponent&>> RenderDataExtractor::extractDirectionLights() const
	{
		auto View = m_Scene->m_Registry.view<TransformComponent, LightComponent>();
		std::vector<std::tuple<TransformComponent&, LightComponent&>> EntityData;
		for (const auto& Entt : View) {
			auto& Light = View.get<LightComponent>(Entt);
			if (dynamic_cast<DirectionLight*>(Light.m_Light.get()))
				EntityData.emplace_back(View.get<TransformComponent, LightComponent>(Entt));
		}
		return EntityData;
	}

	std::vector<std::tuple<TransformComponent&, LightComponent&>> RenderDataExtractor::extractPointLights() const
	{
		auto View = m_Scene->m_Registry.view<TransformComponent, LightComponent>();
		std::vector<std::tuple<TransformComponent&, LightComponent&>> EntityData;
		for (const auto& Entt : View) {
			auto& Light = View.get<LightComponent>(Entt);
			if (dynamic_cast<PointLight*>(Light.m_Light.get()))
				EntityData.emplace_back(View.get<TransformComponent, LightComponent>(Entt));
		}
		return EntityData;
	}

	std::vector<std::tuple<TransformComponent&, LightComponent&>> RenderDataExtractor::extractSpotLights() const
	{
		auto View = m_Scene->m_Registry.view<TransformComponent, LightComponent>();
		std::vector<std::tuple<TransformComponent&, LightComponent&>> EntityData;
		for (const auto& Entt : View) {
			auto& Light = View.get<LightComponent>(Entt);
			if (dynamic_cast<SpotLight*>(Light.m_Light.get()))
				EntityData.emplace_back(View.get<TransformComponent, LightComponent>(Entt));
		}
		return EntityData;
	}

	LightsData RenderDataExtractor::extractLightsData() const
	{
		LightsData Data;
		Data.m_DirectionLights = extractDirectionLights();
		Data.m_PointLights = extractPointLights();
		Data.m_SpotLights = extractSpotLights();
		return Data;
	}

	SceneData RenderDataExtractor::extractSceneData() const
	{
		SceneData Data;
		Data.m_Models = extractComponents<TransformComponent, ModelComponent>();
		return Data;
	}

}
