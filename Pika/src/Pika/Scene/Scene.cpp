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
		auto& Tag = Entity.addComponent<TagComponent>(vName);
		Tag.m_Tag = vName.empty() ? "Entity" : vName;
		Entity.addComponent<TransformComponent>();
		return Entity;

	}
	void Scene::onUpdate(Timestep vTimestep)
	{
		auto View = m_Registry.group<TransformComponent, SpriteRendererComponent>();
		for (auto& Entity : View) {
			auto [Transform, SpriteRenderer] = View.get<TransformComponent, SpriteRendererComponent>(Entity);
			Renderer2D::DrawQuad(Transform.m_Position, Transform.m_Scale, SpriteRenderer.m_Color);
		}
	}
}	