#pragma once
#include "Pika/Core/Timestep.h"
#include <entt.h>

namespace Pika
{
	class Entity; // Forward declaration to prevent recursive references

	class Scene
	{
	public:
		Scene() = default;
		~Scene() = default; // TODO : For now!

		Entity createEntity(const std::string& vName = "");
		//void destroyEntity()

		void onUpdate(Timestep vTimestep);

	private:
		entt::registry m_Registry;

		friend class Entity;
		friend class SceneHierarchyPanel;
	};

}
