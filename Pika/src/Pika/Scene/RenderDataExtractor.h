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

		// Only 2D
		std::vector<std::tuple<TransformComponent&, SpriteRendererComponent&, int>> extractSprites() const;
		// Only 3D
		const Ref<Cubemap>& extractSkybox() const;
		std::vector<Entity> extractDirectionLights() const;
		std::vector<Entity> extractPointLights() const;
		std::vector<Entity> extractSpotLights() const;    // 暂时不直接提取数据
	public:
		Ref<Scene> m_Scene = nullptr;
	};

}
