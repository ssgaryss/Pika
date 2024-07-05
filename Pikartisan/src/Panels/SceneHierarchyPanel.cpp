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
		m_Context->m_Registry.view<TagComponent>().each([this](auto vEntityHandle, auto& vTagComponent) {
			Entity Entity(vEntityHandle, m_Context.get());
			drawEntityNode(Entity);
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
		auto& Tag = vEntity.getComponent<TagComponent>();
		ImGui::TreeNodeEx((void*)(uint64_t)vEntity, ImGuiTreeNodeFlags_Selected, Tag);
	}

	void SceneHierarchyPanel::drawEntityComponents(Entity vEntity)
	{
		// TODO !
		if (vEntity.hasComponent<TagComponent>()) {

		}

		if (vEntity.hasComponent<TransformComponent>()) {
			//ImGui::DragFloat3()
		}

		if (vEntity.hasComponent<CameraComponent>()) {
			//ImGui::BeginCombo()
			//if(perspective)
			//if(orthographic)
		}
	}

}