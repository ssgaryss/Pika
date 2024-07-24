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
		// TODO : UUID
		auto& Tag = Entity.addComponent<TagComponent>(vName);
		Tag.m_Tag = vName.empty() ? "Entity" : vName;
		Entity.addComponent<TransformComponent>();
		return Entity;
	}

	void Scene::destroyEntity(Entity vEntity)
	{
		m_Registry.destroy(vEntity);
	}

	void Scene::onUpdate(Timestep vTimestep)
	{
		auto View = m_Registry.group<TransformComponent, SpriteRendererComponent>();
		for (auto& Entity : View) {
			auto [Transform, SpriteRenderer] = View.get<TransformComponent, SpriteRendererComponent>(Entity); // 此处得到的是tuple，C++17开始对tuple的结构化绑定可以自动推导引用
			// TODO !
			// TODO : 半透明物体需要集中在另一个batch渲染，否则无法blend
			if (Transform.m_Rotation.x == 0 && Transform.m_Rotation.y == 0 && Transform.m_Rotation.z == 0)
				Renderer2D::DrawQuad(Transform.m_Position, Transform.m_Scale, SpriteRenderer.m_Color);
			else
				Renderer2D::DrawRotatedQuad(Transform.m_Position, glm::vec2{ Transform.m_Scale.x,Transform.m_Scale.y },
					Transform.m_Rotation.z, SpriteRenderer.m_Color);
		}
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