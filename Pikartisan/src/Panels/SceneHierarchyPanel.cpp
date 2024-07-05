#include "SceneHierarchyPanel.h"
#include <imgui/imgui.h>
#include <glm/gtc/type_ptr.hpp>

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

	// 主要给drawEntityComponents()使用的模板
	template <typename T, typename Func>
	void drawEntityComponent(const std::string& vName, Entity vEntity, Func vFunction) {
		if (vEntity.hasComponent<T>()) {
			auto& Component = vEntity.getComponent<T>();
			const ImGuiTreeNodeFlags TreeNodeFlags{
				ImGuiTreeNodeFlags_DefaultOpen |
				ImGuiTreeNodeFlags_OpenOnArrow
			};
			// typeid(T).hash_code()保证树节点标号不同
			bool Opened = ImGui::TreeNodeEx((void*)typeid(T).hash_code(), TreeNodeFlags, vName.c_str());
			if (Opened) {
				vFunction(Component);
				ImGui::TreePop();
			}
		}
	}

	void SceneHierarchyPanel::drawEntityComponents(Entity vEntity)
	{
		// TODO !

		drawEntityComponent<TagComponent>("Tag", vEntity, [](auto& vTagComponent) {
			static char Buffer[256];
			memset(Buffer, 0, sizeof(Buffer));
			strcpy_s(Buffer, vTagComponent);

			if (ImGui::InputText("TagComponent", Buffer, sizeof(Buffer)))
				vTagComponent.m_Tag = std::string(Buffer);
			});

		drawEntityComponent<TransformComponent>("Transform", vEntity, [](auto& vTransformComponent) {
			ImGui::DragFloat3("Position", glm::value_ptr(vTransformComponent.m_Position), 0.1f);
			ImGui::DragFloat3("Rotation", glm::value_ptr(vTransformComponent.m_Rotation), 0.1f);
			ImGui::DragFloat3("Scale", glm::value_ptr(vTransformComponent.m_Scale), 0.1f);
			});

		if (vEntity.hasComponent<CameraComponent>()) {
			//ImGui::BeginCombo()
			//if(perspective)
			//if(orthographic)
		}
	}

}