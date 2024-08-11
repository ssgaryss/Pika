#include "pkpch.h"
#include "Scene.h"
#include "Components.h"
#include "Entity.h"
#include "Pika/Renderer/Renderer2D.h"
#include <box2d/box2d.h>

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
		constexpr int32_t VelocityIterations = 6;
		constexpr int32_t PositionIterations = 6;
		m_Physics2DWorld->Step(vTimestep, VelocityIterations, PositionIterations);

		auto View = m_Registry.view<Rigidbody2DComponent>();
		for (const auto& Entt : View) {
			Entity Entity{ Entt, this };
			auto& TC = Entity.getComponent<TransformComponent>();
			auto& R2DC = Entity.getComponent<Rigidbody2DComponent>();

			b2Body* Body = reinterpret_cast<b2Body*>(R2DC.m_RuntimeBody);
			auto& Position = Body->GetPosition();
			TC.m_Position.x = Position.x;
			TC.m_Position.y = Position.y;
			TC.m_Rotation.z = glm::degrees(Body->GetAngle());
		}
	}

	void Scene::onUpdateSimulation(Timestep vTimestep)
	{
		// TODO!!!
	}

	void Scene::onRuntimeBegin()
	{
		onPhysics2DBegin();
	}

	void Scene::onRuntimeEnd()
	{
		onPhysics2DEnd();
	}

	void Scene::onPhysics2DBegin()
	{
		b2Vec2 Gravity{ 0.0f, -9.8f };
		m_Physics2DWorld.reset(new b2World(Gravity));

		auto View = m_Registry.view<Rigidbody2DComponent>();
		for (const auto& Entt : View) {
			Entity Entity{ Entt,this };
			auto& TC = Entity.getComponent<TransformComponent>();
			auto& R2DC = Entity.getComponent<Rigidbody2DComponent>();

			// XOY平面的2D Physics
			b2BodyDef BodyDef;
			BodyDef.type = static_cast<b2BodyType>(R2DC.m_Type);
			BodyDef.position.Set(TC.m_Position.x, TC.m_Position.y);
			BodyDef.angle = glm::radians(TC.m_Rotation.z);

			b2Body* Body = m_Physics2DWorld->CreateBody(&BodyDef);
			Body->SetFixedRotation(R2DC.m_IsFixedRotation);
			Body->SetAwake(true);
			R2DC.m_RuntimeBody = Body;

			if (Entity.hasComponent<BoxCollider2DComponent>()) {
				auto& BC2DC = Entity.getComponent<BoxCollider2DComponent>();
				b2PolygonShape BoxShape;
				BoxShape.SetAsBox(BC2DC.m_Size.x * TC.m_Scale.x, BC2DC.m_Size.y * TC.m_Scale.y,
					b2Vec2{ BC2DC.m_Offset.x, BC2DC.m_Offset.y }, 0.0f);

				b2FixtureDef BoxFixtureDef;
				BoxFixtureDef.shape = &BoxShape;
				BoxFixtureDef.density = BC2DC.m_Density;
				BoxFixtureDef.friction = BC2DC.m_Friction;
				BoxFixtureDef.restitution = BC2DC.m_Restitution;
				BoxFixtureDef.restitutionThreshold = BC2DC.m_RestitutionThreshold;
				Body->CreateFixture(&BoxFixtureDef);
			}
		}
	}

	void Scene::onPhysics2DEnd()
	{
		m_Physics2DWorld = nullptr;
		auto View = m_Registry.view<Rigidbody2DComponent, BoxCollider2DComponent>();
		for (const auto& Entt : View) {
			auto& R2DC = View.get<Rigidbody2DComponent>(Entt);
			auto& BC2DC = View.get<BoxCollider2DComponent>(Entt);
			delete R2DC.m_RuntimeBody;
			R2DC.m_RuntimeBody = nullptr;
			delete BC2DC.m_RuntimeFixture;
			BC2DC.m_RuntimeFixture = nullptr;
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
		for (const auto& Entt : View) {
			auto& Camera = View.get<CameraComponent>(Entt);
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
		for (const auto& Entt : View) {
			const TagComponent& TC = View.get<TagComponent>(Entt);
			if (TC.m_Tag == vName)
				return Pika::Entity{ Entt, this };
		}
		return Entity{};
	}

}