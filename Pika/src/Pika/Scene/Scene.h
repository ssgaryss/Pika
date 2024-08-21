#pragma once
#include "Pika/Core/Timestep.h"
#include "Pika/Renderer/Camera.h"
#include "Pika/Renderer/Texture.h"
#include "Pika/Core/UUID.h"
#include <entt.h>

class b2World;

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
		enum class SceneState {
			Edit = 0,
			Play = 1,
			Simulate = 2
		};
	public:
		Scene() = default;
		Scene(const std::string& vName)
			: m_SceneName{ vName } {};
		Scene(const std::string& vName, SceneType vType)
			: m_SceneName{ vName }, m_SceneType{ vType } {}

		Entity createEntity(const std::string& vName = "Untitled");
		Entity createEntityWithUUID(UUID vUUID, const std::string& vName = "Untitled");
		Entity createEntityWithUUIDString(const std::string& vUUID, const std::string& vName = "Untitled");
		void destroyEntity(Entity& vEntity);

		void onUpdate(Timestep vTiemstep);

		void onViewportResize(uint32_t vWidth, uint32_t vHeight); // 更新Scene和SceneCamera的Viewport

		Entity getEntityByUUID(const UUID& vUUID);
		Entity getEntityByName(std::string_view vName); // 若有重名返回找到的第一个

		inline const std::string& getSceneName() const { return m_SceneName; }
		inline void setSceneName(const std::string& vName) { m_SceneName = vName; }
		inline SceneType getSceneType() const { return m_SceneType; }
		inline void setSceneType(SceneType vSceneType) { m_SceneType = vSceneType; }
	private:
		void onUpdateEditor(Timestep vTimestep);        // SceneState::Edit
		void onUpdateRuntime(Timestep vTimestep);       // SceneState::Play
		void onRuntimeBegin();
		void onRuntimeEnd();
		void onUpdateSimulation(Timestep vTimestep);    // SceneState::Simulate
		void onSimulationBegin();
		void onSimulationEnd();
	private:
		void onPhysics2DBegin();  // 构造m_Physics2DWorld
		void onPhysics2DEnd();    // 析构m_Physics2DWorld

		// TODO!
		//void onSimulationBegin();
		//void onSimulationEnd();

	private:
		std::string m_SceneName = "Untitled";
		entt::registry m_Registry;                                    // Entities
		std::unordered_map<UUID, entt::entity> m_EntityMap;           // UUID -> Entity
		SceneType m_SceneType = SceneType::Scene3D;                   // Scene Type
		SceneState m_SceneState = SceneState::Edit;                   // Scene State
		bool m_IsPaused = false;                                      // Play & Simulation
		uint32_t m_SceneViewportWidth = 0, m_SceneViewportHeight = 0; // Scene Display
		Ref<b2World> m_Physics2DWorld = nullptr;                      // Physics 2D World
		Ref<Cubemap> m_Skybox = nullptr;                              // Skybox

		friend class Entity;
		friend class SceneRenderer;
		friend class SceneHierarchyPanel;
		friend class SceneStatePanel;
		friend class SceneSerializer;
	};

}
