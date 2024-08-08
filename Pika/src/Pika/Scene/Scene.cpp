#include "pkpch.h"
#include "Scene.h"
#include "Components.h"
#include "Entity.h"
#include "Pika/Renderer/Renderer2D.h"

namespace Pika
{

	Entity Scene::createEntity(const std::string& vName)
	{
		return createEntityWithUUID(UUID(), vName);
	}

	Entity Scene::createEntityWithUUID(UUID vUUID, const std::string& vName)
	{
		Entity Entity{ m_Registry.create(), this };
		Entity.addComponent<IDComponent>(vUUID);
		auto& Tag = Entity.addComponent<TagComponent>(vName);
		Tag.m_Tag = vName.empty() ? "Untitled" : vName;
		Entity.addComponent<TransformComponent>();

		m_EntityMap[vUUID] = Entity;
		return Entity;
	}

	Entity Scene::createEntityWithUUIDString(const std::string& vUUID, const std::string& vName)
	{
		return createEntityWithUUID(UUID(vUUID), vName);
	}

	void Scene::destroyEntity(Entity& vEntity)
	{
		m_Registry.destroy(vEntity);
		m_EntityMap.erase(vEntity.getUUID());
	}

	void Scene::onUpdate(Timestep vTimestep)
	{
		switch (m_SceneState)
		{
		case Pika::Scene::SceneState::Edit:
		{
			onUpdateEditor(vTimestep);
			break;
		}
		case Pika::Scene::SceneState::Play:
		{
			onUpdateRuntime(vTimestep);
			break;
		}
		case Pika::Scene::SceneState::Simulate:
		{
			onUpdateSimulation(vTimestep);
			break;
		}
		}
	}

	void Scene::onUpdateEditor(Timestep vTimestep)
	{
		// Editor暂时就是静态场景,没有更新
	}

	void Scene::onUpdateRuntime(Timestep vTimestep)
	{
		// TODO!!!
	}

	void Scene::onUpdateSimulation(Timestep vTimestep)
	{
		// TODO!!!
	}

	void Scene::onViewportResize(uint32_t vWidth, uint32_t vHeight)
	{
		if (m_SceneViewportWidth == vWidth && m_SceneViewportHeight == vHeight)
			return;

		m_SceneViewportWidth = vWidth;
		m_SceneViewportHeight = vHeight;

		// resize camera components with m_IsFixedAspectRatio = false
		auto View = m_Registry.view<CameraComponent>();
		for (auto Entity : View) {
			auto& Camera = View.get<CameraComponent>(Entity);
			if (!Camera.m_IsFixedAspectRatio) {
				Camera.m_Camera.setViewportSize(vWidth, vHeight);
			}
		}
	}

	Entity Scene::getEntityByUUID(const UUID& vUUID)
	{
		return Entity{ m_EntityMap[vUUID], this };
	}

	Entity Scene::getEntityByName(std::string_view vName)
	{
		auto View = m_Registry.view<TagComponent>();
		for (const auto& Entity : View) {
			const TagComponent& TC = View.get<TagComponent>(Entity);
			if (TC.m_Tag == vName)
				return Pika::Entity{ Entity, this };
		}
		return Entity{};
	}

}