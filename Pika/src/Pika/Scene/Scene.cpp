#include "pkpch.h"
#include "Scene.h"
#include "Components.h"
#include "Entity.h"

namespace Pika
{

	Entity Scene::createEntity(const std::string& vName)
	{
		Entity Entity{ m_Registry.create(), this };
		auto& Tag = Entity.addComponent<TagComponent>(vName);
		Tag.m_Tag = vName.empty() ? "Entity" : vName;
		Entity.addComponent<TransformComponent>();
		return Entity;

	}
	void Scene::onUpdate(Timestep vTimestep)
	{
		auto Group = m_Registry.group<TransformComponent>(entt::get<SpriteRendererComponent>);
		//Group.each([]())
	}
}	