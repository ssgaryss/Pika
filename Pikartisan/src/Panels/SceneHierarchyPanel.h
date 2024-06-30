#pragma once
#include <Pika.h>

namespace Pika
{

	class SceneHierarchyPanel
	{
	public:
		SceneHierarchyPanel() = default;
		SceneHierarchyPanel(const Ref<Scene>& vContext);
		inline void setContext(const Ref<Scene>& vContext) { m_Context = vContext; }
		void onImGuiRender();
	private:
		void drawEntityNode(Entity vEntity);        // draw entity node in scene hierarchy panel
		void drawEntityComponents(Entity vEntity);  // draw entity components in scene hierarchy panel
	private:
		Ref<Pika::Scene> m_Context; // Scene data
	};

}
