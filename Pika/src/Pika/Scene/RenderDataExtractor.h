#pragma once
#include "Scene.h"
#include "Entity.h"

namespace Pika {

	class RenderDataExtractor
	{
	public:
		RenderDataExtractor() = default;
		RenderDataExtractor(const Ref<Scene>& vScene)
			: m_Scene{ vScene } {}

		const Ref<Cubemap>& extractSkybox() const;
		std::vector<Entity> extractDirectionLights() const;
		std::vector<Entity> extractPointLights() const;
		std::vector<Entity> extractSpotLights() const;
	private:
		Ref<Scene> m_Scene = nullptr;
	};

}
