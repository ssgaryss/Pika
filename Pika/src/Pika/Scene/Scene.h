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

		void onViewportResize(uint32_t vWidth, uint32_t vHeight); // 更新Scene和SceneCamera的Viewport

	private:
		// Scene data
		entt::registry m_Registry;   // Entities
		// TODO : Lighting & Camera
		Ref<Camera> m_PrimaryCamera; // Camera
	private:
		// Scene display
		uint32_t m_SceneViewportWidth = 0, m_SceneViewportHeight = 0;

		friend class Entity;
		friend class SceneHierarchyPanel;
		friend class SceneSerializer;
	};

}
