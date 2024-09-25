#include "pkpch.h"
#include "RenderDataExtractor.h"
#include "Components.h"
#include "Lights.h"

namespace Pika {


	const Ref<Cubemap>& RenderDataExtractor::extractSkybox() const
	{
		return m_Scene->m_Skybox;
	}

	std::vector<Entity> RenderDataExtractor::extractDirectionLights() const
	{
		auto View = m_Scene->m_Registry.group<LightComponent>();
		std::vector<Entity> DirectionLights;
		for (const auto& Entt : View) {
			auto& Light = m_Scene->m_Registry.get<LightComponent>(Entt);
			if (auto pDirectionLight = dynamic_cast<DirectionLight*>(Light.m_Light.get())) {
				DirectionLights.emplace_back(Entity{ Entt, m_Scene.get() });
			}
		}
		return DirectionLights;
	}

	std::vector<Entity> RenderDataExtractor::extractPointLights() const
	{
		auto View = m_Scene->m_Registry.group<LightComponent>();
		std::vector<Entity> PointLights;
		for (const auto& Entt : View) {
			auto& Light = m_Scene->m_Registry.get<LightComponent>(Entt);
			if (auto pPointLight = dynamic_cast<PointLight*>(Light.m_Light.get())) {
				PointLights.emplace_back(Entity{ Entt, m_Scene.get() });
			}
		}
		return PointLights;
	}

	std::vector<Entity> RenderDataExtractor::extractSpotLights() const
	{
		auto View = m_Scene->m_Registry.group<LightComponent>();
		std::vector<Entity> SpotLights;
		for (const auto& Entt : View) {
			auto& Light = m_Scene->m_Registry.get<LightComponent>(Entt);
			if (auto pSpotLight = dynamic_cast<PointLight*>(Light.m_Light.get())) {
				SpotLights.emplace_back(Entity{ Entt, m_Scene.get() });
			}
		}
		return SpotLights;
	}

}