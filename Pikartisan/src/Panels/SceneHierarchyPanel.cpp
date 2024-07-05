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

		if (m_Context) {
			m_Context->m_Registry.view<TagComponent>().each([this](auto vEntityHandle, auto& vTagComponent) {
				Entity Entity(vEntityHandle, m_Context.get());
				drawEntityNode(Entity);
				});

		}
		ImGui::End();

		ImGui::Begin("Properties");
		if (m_SelectedEntity)
			drawEntityComponents(m_SelectedEntity);
		ImGui::End();
	}

	void SceneHierarchyPanel::drawEntityNode(Entity vEntity)
	{
		auto& Tag = vEntity.getComponent<TagComponent>();
		const ImGuiTreeNodeFlags TreeNodeFlags{
			((m_SelectedEntity == vEntity) ? ImGuiTreeNodeFlags_Selected : 0) |
			ImGuiTreeNodeFlags_OpenOnArrow
		};
		bool Opened = ImGui::TreeNodeEx((void*)(uint64_t)(uint32_t)vEntity, TreeNodeFlags, Tag);
		if (ImGui::IsItemClicked()) {
			m_SelectedEntity = vEntity;
		}
		if (Opened) {
			// TODO : Child Entity need to be added!
			ImGui::TreePop();
		}
	}

	template <typename T, typename Func>
	void drawEntityComponent(const std::string& vName, Entity vEntity, Func vFunction) {
		if (vEntity.hasComponent<T>()) {
			auto& Component = vEntity.getComponent<T>();
			const ImGuiTreeNodeFlags TreeNodeFlags{
				ImGuiTreeNodeFlags_OpenOnArrow
			};
			bool Opened = ImGui::TreeNodeEx((void*)(uint64_t)(uint32_t)vEntity, TreeNodeFlags, vName.c_str());
			if (Opened) {
				vFunction(Component);
				ImGui::TreePop();
			}

		}
	}

	void SceneHierarchyPanel::drawEntityComponents(Entity vEntity)
	{
		// TODO !

		drawEntityComponent<TagComponent>("Tag", vEntity, [](auto& vComponent) {
			static char Buffer[256];
			memset(Buffer, 0, sizeof(Buffer));
			strcpy_s(Buffer, vComponent);

			if (ImGui::InputText("TagComponent", Buffer, sizeof(Buffer)))
				vComponent.m_Tag = std::string(Buffer);
			});

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