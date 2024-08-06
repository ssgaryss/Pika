#pragma once
#include "Pika/Core/Timestep.h"
#include "Pika/Renderer/Camera.h"
#include "Pika/Core/UUID.h"
#include <entt.h>

namespace Pika
{
	class Entity; // Forward declaration to prevent recursive references

	class Scene
	{
	public:
		enum class SceneType {
			Scene2D = 0,
			Scene3D = 1
		};
	public:
		Scene() = default;
		~Scene() = default; // TODO : For now!

		Entity createEntity(const std::string& vName = "Untitled");
		Entity createEntityWithUUID(UUID vUUID, const std::string& vName = "Untitled");
		Entity createEntityWithUUIDString(const std::string& vUUID, const std::string& vName = "Untitled");
		void destroyEntity(Entity vEntity);

		void onUpdateEditor(Timestep vTimestep);
		void onUpdateRuntime(Timestep vTimestep);

		void onViewportResize(uint32_t vWidth, uint32_t vHeight); // 更新Scene和SceneCamera的Viewport

		inline Ref<Camera> getPrimaryCamera() const { return m_PrimaryCamera; }
		inline SceneType getSceneType() const { return m_SceneType; }
		inline void setSceneType(SceneType vSceneType) { m_SceneType = vSceneType; }
	private:
		// Scene data
		entt::registry m_Registry;   // Entities
		// TODO : Lighting & Camera
		Ref<Camera> m_PrimaryCamera; // Camera
	private:
		// Scene type
		SceneType m_SceneType = SceneType::Scene2D; // TODO : 3D
		// Scene display
		uint32_t m_SceneViewportWidth = 0, m_SceneViewportHeight = 0;

		friend class Entity;
		friend class SceneRenderer;
		friend class SceneHierarchyPanel;
		friend class SceneSerializer;
	};

}
