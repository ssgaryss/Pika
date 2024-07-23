#include "SceneHierarchyPanel.h"
#include <format>
#include <imgui/imgui.h>
#include <imgui/imgui_internal.h>
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
			// TODO : Tag -> UUID
			m_Context->m_Registry.view<TagComponent>().each([this](auto vEntity, auto& vTagComponent) {
				Entity Entity(vEntity, m_Context.get());
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
		// TODO : Popup eg.delete entity
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
				ImGuiTreeNodeFlags_SpanAvailWidth |
				ImGuiTreeNodeFlags_Framed
			};

			// typeid(T).hash_code()保证树节点标号不同
			bool Opened = ImGui::TreeNodeEx((void*)typeid(T).hash_code(), TreeNodeFlags, vName.c_str());

			// TODO : 不是所有component都能删除
			if (ImGui::IsItemHovered() && ImGui::IsMouseReleased(ImGuiMouseButton_Right)) {
				ImGui::OpenPopup(std::format("ComponentSettings##{0}", vName).c_str());
			}
			bool IsRemoved = false;
			if (ImGui::BeginPopup(std::format("ComponentSettings##{0}", vName).c_str())) {
				if (ImGui::MenuItem("Delete"))
					IsRemoved = true;
				ImGui::EndPopup();
			}

			if (Opened) {
				vFunction(Component);
				ImGui::TreePop();
			}

			if (IsRemoved)
				vEntity.removeComponent<T>();
		}
	}

	static void drawVec3Control(const std::string& vLabel, glm::vec3& vValue,
		float vResetValue = 0.0f, float vColWidth = 100.0f) {
		ImGuiIO& IO = ImGui::GetIO();
		auto BoldFont = IO.Fonts->Fonts[0]; // TODO : Use a fonts library

		ImGui::PushID(vLabel.c_str());
		ImGui::Columns(2);
		ImGui::SetColumnWidth(0, vColWidth);
		ImGui::Text(vLabel.c_str());
		ImGui::NextColumn();
		ImGui::PushMultiItemsWidths(3, ImGui::CalcItemWidth()); // push 3个ItemWidth
		ImGui::PushStyleVar(ImGuiStyleVar_ItemSpacing, ImVec2{ 0, 0 }); // 不要padding
		float LineHeight = GImGui->Font->FontSize + GImGui->Style.FramePadding.y * 2.0f;
		ImVec2 ButtonSize = { LineHeight + 3.0f, LineHeight };

		ImGui::PushStyleColor(ImGuiCol_Button, ImVec4{ 0.8f, 0.1f, 0.15f, 1.0f });
		ImGui::PushStyleColor(ImGuiCol_ButtonHovered, ImVec4{ 0.9f, 0.2f, 0.2f, 1.0f });
		ImGui::PushStyleColor(ImGuiCol_ButtonActive, ImVec4{ 0.8f, 0.1f, 0.15f, 1.0f });
		ImGui::PushFont(BoldFont);
		if (ImGui::Button("X", ButtonSize))
			vValue.x = vResetValue;
		ImGui::PopFont();
		ImGui::PopStyleColor(3);
		ImGui::SameLine();
		ImGui::DragFloat("##X", &vValue.x, 0.1f);
		ImGui::PopItemWidth(); // pop 第一个

		ImGui::SameLine();
		ImGui::PushStyleColor(ImGuiCol_Button, ImVec4{ 0.2f, 0.7f, 0.2f, 1.0f });
		ImGui::PushStyleColor(ImGuiCol_ButtonHovered, ImVec4{ 0.3f, 0.8f, 0.3f, 1.0f });
		ImGui::PushStyleColor(ImGuiCol_ButtonActive, ImVec4{ 0.2f, 0.7f, 0.2f, 1.0f });
		ImGui::PushFont(BoldFont);
		if (ImGui::Button("Y", ButtonSize))
			vValue.y = vResetValue;
		ImGui::PopFont();
		ImGui::PopStyleColor(3);
		ImGui::SameLine();
		ImGui::DragFloat("##Y", &vValue.y, 0.1f);
		ImGui::PopItemWidth(); // pop 第二个

		ImGui::SameLine();
		ImGui::PushStyleColor(ImGuiCol_Button, ImVec4{ 0.1f, 0.25f, 0.8f, 1.0f });
		ImGui::PushStyleColor(ImGuiCol_ButtonHovered, ImVec4{ 0.2f, 0.35f, 0.9f, 1.0f });
		ImGui::PushStyleColor(ImGuiCol_ButtonActive, ImVec4{ 0.1f, 0.25f, 0.8f, 1.0f });
		ImGui::PushFont(BoldFont);
		if (ImGui::Button("Z", ButtonSize))
			vValue.z = vResetValue;
		ImGui::PopFont();
		ImGui::PopStyleColor(3);
		ImGui::SameLine();
		ImGui::DragFloat("##Z", &vValue.z, 0.1f);
		ImGui::PopItemWidth(); // pop 第三个
		ImGui::PopStyleVar();

		ImGui::Columns(1); // 设置回默认的单列
		ImGui::PopID();
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
			drawVec3Control("Translation", vTransformComponent.m_Position);
			drawVec3Control("Rotation", vTransformComponent.m_Rotation);
			drawVec3Control("Scale", vTransformComponent.m_Scale, 1.0f);
			});

		drawEntityComponent<SpriteRendererComponent>("Sprite Renderer", vEntity, [](auto& vSpriteRendererComponent) {
			ImGui::ColorEdit4("##Color", glm::value_ptr(vSpriteRendererComponent.m_Color));
			});

		drawEntityComponent<CameraComponent>("Camera", vEntity, [](auto& vCameraComponent) {
			//TODO!
			float OthograhicSize = vCameraComponent.m_Camera.getOthographicSize();
			ImGui::DragFloat("aa", &OthograhicSize);
			});

	}

}