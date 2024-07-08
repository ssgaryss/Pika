#include "SceneHierarchyPanel.h"
#include <format>
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

			if (ImGui::BeginPopupContextWindow("Create entity", ImGuiPopupFlags_MouseButtonRight)) {
				if (ImGui::MenuItem("Empty entity"))
					m_Context->createEntity("Empty entity");
				ImGui::EndPopup();
			}

		}
		ImGui::End();

		ImGui::Begin("Properties");
		if (m_SelectedEntity) {
			drawEntityComponents(m_SelectedEntity);
			if (ImGui::Button("Add component"))
				ImGui::OpenPopup("AddComponent");

			if (ImGui::BeginPopup("AddComponent")) {
				if (!m_SelectedEntity.hasComponent<SpriteRendererComponent>()) {
					if (ImGui::MenuItem("Sprite Renderer Component")) {
						m_SelectedEntity.addComponent<SpriteRendererComponent>();
					}
				}
				if (!m_SelectedEntity.hasComponent<CameraComponent>()) {
					if (ImGui::MenuItem("Camera Component")) {
						m_SelectedEntity.addComponent<CameraComponent>();
					}
				}
				ImGui::EndPopup();
			}
		}

		ImGui::End();
	}

	void SceneHierarchyPanel::drawEntityNode(Entity vEntity)
	{
		auto& Tag = vEntity.getComponent<TagComponent>();
		const ImGuiTreeNodeFlags TreeNodeFlags{
			((m_SelectedEntity == vEntity) ? ImGuiTreeNodeFlags_Selected : 0) |
			ImGuiTreeNodeFlags_OpenOnArrow |
			ImGuiTreeNodeFlags_SpanAvailWidth
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
	template <typename T, typename UIFunction>
	void drawEntityComponent(const std::string& vName, Entity vEntity, UIFunction vFunction) {
		if (vEntity.hasComponent<T>()) {
			auto& Component = vEntity.getComponent<T>();
			const ImGuiTreeNodeFlags TreeNodeFlags{
				ImGuiTreeNodeFlags_DefaultOpen |
				ImGuiTreeNodeFlags_OpenOnArrow |
				ImGuiTreeNodeFlags_SpanAvailWidth
			};
			// typeid(T).hash_code()保证树节点标号不同
			bool Opened = ImGui::TreeNodeEx((void*)typeid(T).hash_code(), TreeNodeFlags, vName.c_str());
			if (Opened) {
				vFunction(Component);
				ImGui::TreePop();
			}

			if (ImGui::IsItemHovered() && ImGui::IsMouseReleased(ImGuiMouseButton_Right)) {
				ImGui::OpenPopup(std::format("ComponentSettings##{0}", vName).c_str());
			}

			bool IsRemoved = false;
			if (ImGui::BeginPopup(std::format("ComponentSettings##{0}", vName).c_str())) {
				if (ImGui::MenuItem("Delete"))
					IsRemoved = true;
				ImGui::EndPopup();
			}

			if (IsRemoved)
				vEntity.removeComponent<T>();
		}
	}

	void SceneHierarchyPanel::drawEntityComponents(Entity vEntity)
	{
		// TODO !

		drawEntityComponent<TagComponent>("Tag", vEntity, [](auto& vTagComponent) {
			static char Buffer[256];
			memset(Buffer, 0, sizeof(Buffer));
			strcpy_s(Buffer, vTagComponent);

			if (ImGui::InputText("##TagComponent", Buffer, sizeof(Buffer)))
				vTagComponent.m_Tag = std::string(Buffer);
			});

		drawEntityComponent<TransformComponent>("Transform", vEntity, [](auto& vTransformComponent) {
			ImGui::DragFloat3("##Position", glm::value_ptr(vTransformComponent.m_Position), 0.1f);
			ImGui::DragFloat3("##Rotation", glm::value_ptr(vTransformComponent.m_Rotation), 0.1f);
			ImGui::DragFloat3("##Scale", glm::value_ptr(vTransformComponent.m_Scale), 0.1f);
			});

		drawEntityComponent<SpriteRendererComponent>("Sprite Renderer", vEntity, [](auto& vSpriteRendererComponent) {
			ImGui::ColorEdit4("Color", glm::value_ptr(vSpriteRendererComponent.m_Color));
			});

		drawEntityComponent<CameraComponent>("Camera", vEntity, [](auto& vCameraComponent) {

			});

	}

}