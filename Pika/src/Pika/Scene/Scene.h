#pragma once
#include <entt.h>

namespace Pika
{

	class Scene
	{
	public:
		Scene();
		~Scene();

		//entt::registry get
	private:
		entt::registry m_Registry;
	};

}
