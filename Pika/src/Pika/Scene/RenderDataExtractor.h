#pragma once
#include "Scene.h"
#include "Entity.h"
#include "Components.h"

namespace Pika {

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
			auto View = m_Scene->m_Registry.group<Components...>();
			std::vector<std::tuple<Components&..., int>> EntityData;
			// 遍历实体，提取相关组件并将它们与实体 ID 一起存入结果
			for (const auto& Entt : View) {
				auto RequiredComponents = m_Scene->m_Registry.get<Components...>(Entt);
				EntityData.emplace_back(std::tuple_cat(RequiredComponents, std::make_tuple(static_cast<int>(Entt))));
			}
			return EntityData;
		}

		template<typename... Components>
		std::vector<std::tuple<Components&...>> extractComponents() const {
			auto View = m_Scene->m_Registry.group<Components...>();
			std::vector<std::tuple<Components&...>> EntityData;
			for (const auto& Entt : View)
				EntityData.emplace_back(m_Scene->m_Registry.get<Components...>(Entt));
			return EntityData;
		}

		// Only 3D
		const Ref<Cubemap>& extractSkybox() const;
		std::vector<std::tuple<TransformComponent&, ModelComponent&, int>> extractNoMaterialModels() const;
		std::vector<Entity> extractDirectionLights() const;
		std::vector<Entity> extractPointLights() const;
		std::vector<Entity> extractSpotLights() const;    // 暂时不直接提取数据

	public:
		Ref<Scene> m_Scene = nullptr;
	};

}
