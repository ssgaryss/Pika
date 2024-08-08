#pragma once
#include <Pika.h>

namespace Pika
{

	class SceneHierarchyPanel
	{
	public:
		SceneHierarchyPanel() = default;
		SceneHierarchyPanel(const Ref<Scene>& vContext);
		inline void setContext(const Ref<Scene>& vContext) { m_Context = vContext; m_SelectedEntity = {}; }
		inline const Entity getSelectedEntity() const { return m_SelectedEntity; }
		inline void setSelectedEntity(const Entity& vSelectedEntity) { m_SelectedEntity = vSelectedEntity; }
		inline bool* getIsShowSceneHirarchy() { return &m_IsShowSceneHirarchy; }

		void onImGuiRender();
	private:
		void drawEntityNode(Entity vEntity);        // draw entity node in scene hierarchy panel
		void drawEntityComponents(Entity vEntity);  // draw entity components in scene hierarchy panel
	private:
		Ref<Scene> m_Context = nullptr; // Scene data
		Entity m_SelectedEntity;
		bool m_IsShowSceneHirarchy = true;
	};

}
