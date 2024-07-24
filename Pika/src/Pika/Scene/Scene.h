#pragma once
#include "Pika/Core/Timestep.h"
#include "Pika/Renderer/Camera.h"
#include <entt.h>

namespace Pika
{
	class Entity; // Forward declaration to prevent recursive references

	class Scene
	{
	public:
		Scene() = default;
		~Scene() = default; // TODO : For now!

		Entity createEntity(const std::string& vName = "Untitled");
		void destroyEntity(Entity vEntity);

		void onUpdate(Timestep vTimestep);

	private:
		entt::registry m_Registry;   // Entities
		// TODO : Lighting & Camera
		Ref<Camera> m_PrimaryCamera; // Camera

		friend class Entity;
		friend class SceneHierarchyPanel;
		friend class SceneSerializer;
	};

}
