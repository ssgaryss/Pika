#include "pkpch.h"
#include "Entity.h"

namespace Pika
{
	Entity::Entity(entt::entity vEntityHandle, Scene* vScene)
		: m_EntityHandle{ vEntityHandle }, m_Scene{ vScene }
	{
	}
}