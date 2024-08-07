#include "pkpch.h"
#include "Scene.h"
#include "Components.h"
#include "Entity.h"
#include "Pika/Renderer/Renderer2D.h"

namespace Pika
{

	Entity Scene::createEntity(const std::string& vName)
	{
		Entity Entity{ m_Registry.create(), this };
		Entity.addComponent<IDComponent>();
		auto& Tag = Entity.addComponent<TagComponent>(vName);
		Tag.m_Tag = vName.empty() ? "Untitled" : vName;
		Entity.addComponent<TransformComponent>();
		return Entity;
	}

	Entity Scene::createEntityWithUUID(UUID vUUID, const std::string& vName)
	{
		Entity Entity{ m_Registry.create(), this };
		Entity.addComponent<IDComponent>(vUUID);
		auto& Tag = Entity.addComponent<TagComponent>(vName);
		Tag.m_Tag = vName.empty() ? "Untitled" : vName;
		Entity.addComponent<TransformComponent>();
		return Entity;
	}

	Entity Scene::createEntityWithUUIDString(const std::string& vUUID, const std::string& vName)
	{
		Entity Entity{ m_Registry.create(), this };
		Entity.addComponent<IDComponent>(vUUID);
		auto& Tag = Entity.addComponent<TagComponent>(vName);
		Tag.m_Tag = vName.empty() ? "Untitled" : vName;
		Entity.addComponent<TransformComponent>();
		return Entity;
	}

	void Scene::destroyEntity(const Entity& vEntity)
	{
		m_Registry.destroy(vEntity);
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
}