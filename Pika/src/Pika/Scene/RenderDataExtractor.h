#pragma once
#include "Scene.h"
#include "Entity.h"
#include "Components.h"
#include "Mesh.h"

namespace Pika {


	struct LightsData
	{
		std::vector<std::tuple<TransformComponent&, LightComponent&>> m_DirectionLights;
		std::vector<std::tuple<TransformComponent&, LightComponent&>> m_PointLights;
		std::vector<std::tuple<TransformComponent&, LightComponent&>> m_SpotLights;
		LightsData() {
			m_DirectionLights.reserve(4);
			m_PointLights.reserve(8);
			m_SpotLights.reserve(8); // 防止vector扩容增大开销
		}

		bool empty() const {
			return m_DirectionLights.empty() && m_PointLights.empty() && m_SpotLights.empty();
		}
	};

	struct SceneData
	{
		std::vector<std::tuple<TransformComponent&, ModelComponent&>> m_Models;
		SceneData() {
			m_Models.reserve(10); // 防止vector扩容增大开销
		}

		bool empty() const {
			return m_Models.empty();
		}
	};

	class RenderDataExtractor
	{
	public:
		RenderDataExtractor() = default;
		RenderDataExtractor(const Ref<Scene>& vScene)
			: m_Scene{ vScene } {}

		inline const Ref<Scene>& getScene() const { return m_Scene; }
		inline void setScene(const Ref<Scene>& vScene) { m_Scene = vScene; }

		Scene::SceneType extractSceneType() const;

		template<typename... Components>
		std::vector<std::tuple<Components&..., int>> extractComponentsWithEntityID() const {
			auto View = m_Scene->m_Registry.view<Components...>();
			std::vector<std::tuple<Components&..., int>> EntityData;
			// 遍历实体，提取相关组件并将它们与实体 ID 一起存入结果
			for (const auto& Entt : View) {
				auto RequiredComponents = View.get<Components...>(Entt);
				EntityData.emplace_back(std::tuple_cat(RequiredComponents, std::make_tuple(static_cast<int>(Entt))));
			}
			return EntityData;
		}

		template<typename... Components>
		std::vector<std::tuple<Components&...>> extractComponents() const {
			auto View = m_Scene->m_Registry.view<Components...>();
			std::vector<std::tuple<Components&...>> EntityData;
			for (const auto& Entt : View)
				EntityData.emplace_back(View.get<Components...>(Entt));
			return EntityData;
		}

		// Only 3D
		const Ref<Cubemap>& extractSkybox() const;
		std::vector<std::tuple<TransformComponent&, ModelComponent&, int>> extractNoMaterialModelsWithEntityID() const;
		std::vector<std::tuple<TransformComponent&, ModelComponent&, MaterialComponent&, int>> extractBlinnPhoneMaterialModelsWithEntityID() const;
		std::vector<std::tuple<TransformComponent&, LightComponent&>> extractDirectionLights() const;
		std::vector<std::tuple<TransformComponent&, LightComponent&>> extractPointLights() const;
		std::vector<std::tuple<TransformComponent&, LightComponent&>> extractSpotLights() const;    // 暂时不直接提取数据
		LightsData extractLightsData() const;
		SceneData extractSceneData() const;

	private:
		Ref<Scene> m_Scene = nullptr;
	};


}
