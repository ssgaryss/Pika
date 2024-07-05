#pragma once
#include "Scene.h"
#include <entt.h>

namespace Pika
{

	class Entity
	{
	public:
		Entity() = default;
		Entity(entt::entity vEntityHandle, Scene* vScene);
		Entity(const Entity&) = default;

		template<typename T, typename... Args>
		T& addComponent(Args&&... vArgs) {
			auto& Component = m_Scene->m_Registry.emplace<T>(m_EntityHandle, std::forward<Args>(vArgs)...);
			return Component;
		}

		template<typename T, typename... Args>
		T& addOrReplaceComponent(Args&&... vArgs) {
			auto& Component = m_Scene->m_Registry.emplace_or_replace<T>(m_EntityHandle, std::forward<Args>(vArgs)...);
			return Component;
		}

		template<typename T>
		bool hasComponent() {
			return m_Scene->m_Registry.all_of<T>(m_EntityHandle);
		}

		template<typename T>
		T& getComponent() {
			PK_CORE_ASSERT(hasComponent<T>(), "Entity : Get a component which is not exist!");
			auto& Component = m_Scene->m_Registry.get<T>(m_EntityHandle);
			return Component;
		}

		template<typename T>
		void removeComponent() {
			PK_CORE_ASSERT(hasComponent<T>(), "Entity : Remove a component which is not exist!");
			m_Scene->m_Registry.remove<T>(m_EntityHandle);
		}

		operator bool() const { return m_EntityHandle != entt::null; }
		operator uint32_t() const { return static_cast<uint32_t>(m_EntityHandle); }
		bool operator==(const Entity& vOther) const { return m_EntityHandle == vOther.m_EntityHandle; }

	private:
		entt::entity m_EntityHandle = entt::null;
		Scene* m_Scene = nullptr; // TODO : remove raw pointer
	};

}
