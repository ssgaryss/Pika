#include "SceneHierarchyPanel.h"
#include <imgui/imgui.h>

namespace Pika {

	SceneHierarchyPanel::SceneHierarchyPanel(const Ref<Scene>& vContext)
	{
		setContext(vContext);
	}

	void SceneHierarchyPanel::onImGuiRender()
	{
		ImGui::Begin("Scene Hierarchy");
		m_Context->m_Registry.view<TagComponent>().each([this](auto vEntityHandle) {
			Entity Entity{ vEntityHandle, m_Context.get() };
			drawEntityNode();
			});
		ImGui::End();

		ImGui::Begin("Properties");
		//if(m_SelectionContext)
			//drawEntityComponents();
		ImGui::End();
	}

	void SceneHierarchyPanel::drawEntityNode(Entity vEntity)
	{
		//TODO : ImGui TreeNodeEx!
	}

	void SceneHierarchyPanel::drawEntityComponents(Entity vEntity)
	{
		// TODO !
		if (vEntity.hasComponent<TagComponent>()){

		}

		if (vEntity.hasComponent<TransformComponent>()){
			//ImGui::DragFloat3()
		}

		if (vEntity.hasComponent<CameraComponent>()) {
			//ImGui::BeginCombo()
			//if(perspective)
			//if(orthographic)
		}
	}

}