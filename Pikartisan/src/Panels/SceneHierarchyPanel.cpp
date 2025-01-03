#include "SceneHierarchyPanel.h"
#include <format>
#include <imgui/imgui.h>
#include <imgui/imgui_internal.h>
#include <glm/gtc/type_ptr.hpp>

namespace Pika {


	SceneHierarchyPanel::SceneHierarchyPanel()
	{
		m_DefaultTexture = Texture2D::Create("resources/icons/ComponentPanel/DefaultTexture.png");
	}

	SceneHierarchyPanel::SceneHierarchyPanel(const Ref<Scene>& vContext)
	{
		setContext(vContext);
		m_DefaultTexture = Texture2D::Create("resources/icons/ComponentPanel/DefaultTexture.png");
	}

	void SceneHierarchyPanel::onImGuiRender()
	{
		if (m_IsShowSceneHirarchy) {
			ImGui::Begin("Scene Hierarchy", &m_IsShowSceneHirarchy);

			if (m_Context) {
				// TODO : Tag -> UUID
				m_Context->m_Registry.view<IDComponent>().each([this](auto vEntity, auto& vTagComponent) {
					Entity Entity(vEntity, m_Context.get());
					drawEntityNode(Entity);
					});

				// 左键空白区域取消选中
				if (ImGui::IsMouseDown(ImGuiMouseButton_Left) && ImGui::IsWindowHovered())
					m_SelectedEntity = {};

				// 右键空白区域弹出菜单
				if (ImGui::BeginPopupContextWindow("Create entity", ImGuiPopupFlags_MouseButtonRight | ImGuiPopupFlags_NoOpenOverItems)) {
					if (ImGui::BeginMenu("Create Entity")) {
						if (ImGui::MenuItem("Empty entity"))
							m_Context->createEntity("Empty entity");
						// Only 3D
						if (m_Context->getSceneType() == Scene::SceneType::Scene3D) {
							if (ImGui::BeginMenu("Light")) {
								if (ImGui::MenuItem("Point Light")) {
									auto Entity = m_Context->createEntity("Point Light");
									auto& LC = Entity.addComponent<LightComponent>();
									LC.m_Light = CreateRef<PointLight>();
								}
								if (ImGui::MenuItem("Direction Light")) {
									auto Entity = m_Context->createEntity("Direction Light");
									auto& LC = Entity.addComponent<LightComponent>();
									LC.m_Light = CreateRef<DirectionLight>();
								}
								ImGui::EndMenu();
							}
							if (ImGui::BeginMenu("Mesh")) {
								if (ImGui::MenuItem("Plane")) {
									auto Entity = m_Context->createEntity("Plane");
									auto& MC = Entity.addComponent<ModelComponent>();
									MC.m_Model = CreateRef<Model>("resources/objects/plane.fbx");
								}
								if (ImGui::MenuItem("Sphere")) {
									auto Entity = m_Context->createEntity("Sphere");
									auto& MC = Entity.addComponent<ModelComponent>();
									MC.m_Model = CreateRef<Model>("resources/objects/sphere.fbx");
								}
								if (ImGui::MenuItem("Cube")) {
									auto Entity = m_Context->createEntity("Cube");
									auto& MC = Entity.addComponent<ModelComponent>();
									MC.m_Model = CreateRef<Model>("resources/objects/cube.fbx");
								}
								if (ImGui::MenuItem("Cone")) {
									auto Entity = m_Context->createEntity("Cone");
									auto& MC = Entity.addComponent<ModelComponent>();
									MC.m_Model = CreateRef<Model>("resources/objects/cone.fbx");
								}
								if (ImGui::MenuItem("Cylinder")) {
									auto Entity = m_Context->createEntity("Cylinder");
									auto& MC = Entity.addComponent<ModelComponent>();
									MC.m_Model = CreateRef<Model>("resources/objects/cylinder.fbx");
								}
								if (ImGui::MenuItem("Monkey")) {
									auto Entity = m_Context->createEntity("Monkey");
									auto& MC = Entity.addComponent<ModelComponent>();
									MC.m_Model = CreateRef<Model>("resources/objects/monkey.fbx");
								}
								ImGui::EndMenu();
							}
						}
						// Only 2D
						if (m_Context->getSceneType() == Scene::SceneType::Scene2D) {
							if (ImGui::MenuItem("Sprite")) {
								auto Entity = m_Context->createEntity("Sprite");
								auto SRC = Entity.addComponent<SpriteRendererComponent>();
							}
						}
						ImGui::EndMenu();
					}
					ImGui::EndPopup();
				}

			}
			ImGui::End();

			ImGui::Begin("Properties");
			if (m_SelectedEntity) {
				drawEntityComponents(m_SelectedEntity);
				ImGui::Separator();
			}

			ImGui::End();
		}
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
		if (ImGui::BeginDragDropSource()) {
			const UUID& ID = vEntity.getUUID();
			ImGui::SetDragDropPayload("SCENE_CAMERA", &ID, sizeof(ID));
			ImGui::EndDragDropSource();
		}

		// 左键单击选中Entity
		if (ImGui::IsItemClicked(ImGuiMouseButton_Left))
			m_SelectedEntity = vEntity;
		// TODO : 左键双击Focus

		if (ImGui::IsItemHovered() && ImGui::IsMouseReleased(ImGuiMouseButton_Right))
			ImGui::OpenPopup(std::format("EntitySettings##{0}", (uint32_t)vEntity).c_str());

		bool IsRemove = false;
		if (ImGui::BeginPopup(std::format("EntitySettings##{0}", (uint32_t)vEntity).c_str())) {
			if (ImGui::MenuItem("Delete")) {
				IsRemove = true;
				if (m_SelectedEntity == vEntity)
					m_SelectedEntity = {};
			}
			ImGui::EndPopup();
		}
		ImGui::SetItemTooltip("Right-click to open popup");

		if (Opened) {
			// TODO : Child Entity need to be added!
			ImGui::TreePop();
		}

		if (IsRemove)
			m_Context->destroyEntity(vEntity);
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

			ImGui::PushStyleVar(ImGuiStyleVar_FramePadding, ImVec2(4, 4));
			// typeid(T).hash_code()保证树节点标号不同
			bool Opened = ImGui::TreeNodeEx((void*)typeid(T).hash_code(), TreeNodeFlags, vName.c_str());
			ImGui::PopStyleVar();

			// TODO : 不是所有component都能删除
			if (ImGui::IsItemHovered() && ImGui::IsMouseReleased(ImGuiMouseButton_Right)) {
				ImGui::OpenPopup(std::format("ComponentSettings##{0}", vName).c_str());
			}
			bool IsRemoved = false;
			if (ImGui::BeginPopup(std::format("ComponentSettings##{0}", vName).c_str())) {
				if (!std::is_same<T, TransformComponent>::value && !std::is_same<T, TagComponent>::value) {
					if (ImGui::MenuItem("Delete"))
						IsRemoved = true;
				}
				ImGui::EndPopup();
			}
			ImGui::SetItemTooltip("Right-click to open popup");

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

		ImGui::Columns(); // 设置回默认的单列
		ImGui::PopID();
	}

	void SceneHierarchyPanel::drawEntityComponents(Entity vEntity)
	{
		drawEntityComponent<TagComponent>("Tag", vEntity, [](auto& vTagComponent) {
			static char Buffer[256];
			memset(Buffer, 0, sizeof(Buffer));
			strcpy_s(Buffer, vTagComponent);

			if (ImGui::InputText("##TagComponent", Buffer, sizeof(Buffer)))
				vTagComponent.m_Tag = std::string(Buffer);
			});

		// 点击AddComponent按钮弹出加入component弹窗
		ImGui::SameLine();
		ImGui::PushItemWidth(-1);
		if (ImGui::Button("Add component"))
			ImGui::OpenPopup("AddComponent");
		if (ImGui::BeginPopup("AddComponent")) {
			if (!m_SelectedEntity.hasComponent<CameraComponent>()) {
				if (ImGui::MenuItem("Camera Component")) {
					m_SelectedEntity.addComponent<CameraComponent>();
				}
			}
			// Only 2D
			if (m_Context->getSceneType() == Scene::SceneType::Scene2D) {
				if (!m_SelectedEntity.hasComponent<SpriteRendererComponent>()) {
					if (ImGui::MenuItem("Sprite Renderer Component")) {
						m_SelectedEntity.addComponent<SpriteRendererComponent>();
					}
				}
				if (!m_SelectedEntity.hasComponent<Rigidbody2DComponent>()) {
					if (ImGui::MenuItem("Rigidbody2D Component")) {
						m_SelectedEntity.addComponent<Rigidbody2DComponent>();
					}
				}
				if (!m_SelectedEntity.hasComponent<BoxCollider2DComponent>()) {
					if (ImGui::MenuItem("BoxCollider2D Component")) {
						m_SelectedEntity.addComponent<BoxCollider2DComponent>();
					}
				}
			}
			// Only 3D
			if (m_Context->getSceneType() == Scene::SceneType::Scene3D) {
				if (!m_SelectedEntity.hasComponent<ModelComponent>()) {
					if (ImGui::MenuItem("Model Component")) {
						m_SelectedEntity.addComponent<ModelComponent>();
					}
				}
				if (!m_SelectedEntity.hasComponent<MaterialComponent>()) {
					if (ImGui::BeginMenu("Material Component")) {
						if (ImGui::BeginMenu("Blinn-Phone")) {
							if (ImGui::MenuItem("Defualt")) {
								auto& MC = m_SelectedEntity.addComponent<MaterialComponent>();
								MC.m_Material = CreateRef<BlinnPhoneMaterial>();
							}
							if (ImGui::MenuItem("Gold")) {
								auto& MC = m_SelectedEntity.addComponent<MaterialComponent>();
								MC.m_Material = CreateRef<BlinnPhoneMaterial>(BlinnPhoneMaterial::Data{
									{  0.24725,   0.1995,   0.0745 },
									{  0.75164,  0.60648,  0.22648 },
									{ 0.628281, 0.555802, 0.366065 },
									0.4f
									});
							}
							if (ImGui::MenuItem("Pearl")) {
								auto& MC = m_SelectedEntity.addComponent<MaterialComponent>();
								MC.m_Material = CreateRef<BlinnPhoneMaterial>(BlinnPhoneMaterial::Data{
									{     0.25,	0.20725, 0.20725 },
									{        1,	  0.829,   0.829 },
									{ 0.296648,0.296648,0.296648 },
									0.088f
									});
							}
							if (ImGui::MenuItem("Jade")) {
								auto& MC = m_SelectedEntity.addComponent<MaterialComponent>();
								MC.m_Material = CreateRef<BlinnPhoneMaterial>(BlinnPhoneMaterial::Data{
									{    0.135,	 0.2225,  0.1575 },
									{     0.54,	   0.89,	0.63 },
									{ 0.316228,0.316228,0.316228 },
									0.1f
									});
							}
							ImGui::EndMenu();
						}
						ImGui::EndMenu();
					}
				}
				if (!m_SelectedEntity.hasComponent<LightComponent>()) {
					if (ImGui::BeginMenu("Light Component")) {
						if (ImGui::MenuItem("Point Light")) {
							auto& LC = m_SelectedEntity.addComponent<LightComponent>();
							LC.m_Light = CreateRef<PointLight>();
						}
						ImGui::EndMenu();
					}
				}
			}
			ImGui::EndPopup();
		}
		ImGui::PopItemWidth();

		drawEntityComponent<TransformComponent>("Transform", vEntity, [](auto& vTransformComponent) {
			drawVec3Control("Translation", vTransformComponent.m_Position);
			drawVec3Control("Rotation", vTransformComponent.m_Rotation);
			drawVec3Control("Scale", vTransformComponent.m_Scale, 1.0f);
			});


		drawEntityComponent<CameraComponent>("Camera", vEntity, [](auto& vCameraComponent) {
			Camera::CameraProjectionMode CurrentProjectionMode = vCameraComponent.m_Camera.getProjectionMode();
			int Index = static_cast<int>(CurrentProjectionMode);
			const char* ProjectionModes[] = { "Othographic", "Perspective" };
			if (ImGui::Combo("##ProjectionMode", &Index, ProjectionModes, IM_ARRAYSIZE(ProjectionModes))) {
				vCameraComponent.m_Camera.setProjectionMode(static_cast<Camera::CameraProjectionMode>(Index));
			}
			ImGui::Columns(2);
			ImGui::SetColumnWidth(0, 125.0f);
			if (CurrentProjectionMode == Camera::CameraProjectionMode::Othographic) {
				float OthographicSize = vCameraComponent.m_Camera.getOthographicSize();
				float OthographicNear = vCameraComponent.m_Camera.getOthographicNear();
				float OthographicFar = vCameraComponent.m_Camera.getOthographicFar();
				ImGui::Text("Othographic Size");
				ImGui::NextColumn();
				ImGui::DragFloat("##OthographicSize", &OthographicSize, 0.1f, 0.1f, 1000.0f);
				ImGui::NextColumn();
				ImGui::Text("Othographic Near");
				ImGui::NextColumn();
				ImGui::DragFloat("##OthographicNear", &OthographicNear, 0.1f, 0.1f, OthographicFar - 0.5f);
				ImGui::NextColumn();
				ImGui::Text("Othographic Far");
				ImGui::NextColumn();
				ImGui::DragFloat("##OthographicFar", &OthographicFar, 0.1f, OthographicNear + 0.5f, 100000.0f);
				vCameraComponent.m_Camera.setOthographic(OthographicSize, OthographicNear, OthographicFar);
			}
			else if (CurrentProjectionMode == Camera::CameraProjectionMode::Perspective) {
				float PerspectiveFOV = vCameraComponent.m_Camera.getPerspectiveFOV();
				float PerspectiveNear = vCameraComponent.m_Camera.getPerspectiveNear();
				float PerspectiveFar = vCameraComponent.m_Camera.getPerspectiveFar();
				ImGui::Text("Perspective FOV");
				ImGui::NextColumn();
				ImGui::DragFloat("##PerspectiveFOV", &PerspectiveFOV, 0.1f, 10.0f, 150.0f);
				ImGui::NextColumn();
				ImGui::Text("Perspective Near");
				ImGui::NextColumn();
				ImGui::DragFloat("##PerspectiveNear", &PerspectiveNear, 0.1f, 0.1f, PerspectiveFar - 0.5f);
				ImGui::NextColumn();
				ImGui::Text("Perspective Far");
				ImGui::NextColumn();
				ImGui::DragFloat("##PerspectiveFar", &PerspectiveFar, 0.1f, PerspectiveNear + 0.5f, 100000.0f);
				vCameraComponent.m_Camera.setPerspective(PerspectiveFOV, PerspectiveNear, PerspectiveFar);
			}
			ImGui::Columns();
			});

		// Only 2D 
		if (m_Context->getSceneType() == Scene::SceneType::Scene2D) {
			drawEntityComponent<SpriteRendererComponent>("Sprite Renderer", vEntity, [this](auto& vSpriteRendererComponent) {
				uintptr_t Texture = vSpriteRendererComponent.m_Texture ? static_cast<uintptr_t>(vSpriteRendererComponent.m_Texture->getRendererID()) 
					: static_cast<uintptr_t>(m_DefaultTexture->getRendererID());

				ImGui::Columns(2);
				ImGui::SetColumnWidth(0, 100.0f);
				ImGui::Text("Texture");
				ImGui::NextColumn();
				ImGui::ImageButton(reinterpret_cast<ImTextureID>(Texture), { 50.0f, 50.0f }, { 0, 1 }, { 1, 0 });
				if (ImGui::BeginDragDropTarget()) {
					if (const ImGuiPayload* Payload = ImGui::AcceptDragDropPayload("CONTENT_BROWSER_ITEM")) { // 对应ContentBrowserPanel中
						std::filesystem::path Path{ reinterpret_cast<const wchar_t*>(Payload->Data) };
						const auto& Texture2D = Texture2D::Create(Path);
						vSpriteRendererComponent.m_Texture = Texture2D->getIsLoaded() ? Texture2D : nullptr;
					}
					ImGui::EndDragDropTarget();
				}
				if (ImGui::IsItemHovered() && ImGui::IsMouseReleased(ImGuiMouseButton_Right)) {
					ImGui::OpenPopup("SpriteTexture");
				}
				if (ImGui::BeginPopup("SpriteTexture")) {
					if (ImGui::MenuItem("Delete"))
						vSpriteRendererComponent.m_Texture = nullptr;
					ImGui::EndPopup();
				}
				ImGui::NextColumn();
				ImGui::Text("Tile Factor");
				ImGui::NextColumn();
				ImGui::DragFloat2("##Tiling Factor", glm::value_ptr(vSpriteRendererComponent.m_TilingFactor), 0.05f, 0.0f, 100000.0f);
				ImGui::NextColumn();
				ImGui::Text("Color");
				ImGui::NextColumn();
				ImGui::ColorEdit4("##Color", glm::value_ptr(vSpriteRendererComponent.m_Color));
				ImGui::NextColumn();
				ImGui::Columns();
				});
			drawEntityComponent<Rigidbody2DComponent>("Rigidbody2D Component", vEntity, [](auto& vRigidbody2DComponent) {
				Rigidbody2DComponent::RigidbodyType CurrentRigidbodyType = vRigidbody2DComponent.m_Type;
				int Index = static_cast<int>(CurrentRigidbodyType);
				const char* RigidbodyType[] = { "Static", "Kinematic", "Dynamic" };
				if (ImGui::Combo("##RigidbodyType", &Index, RigidbodyType, IM_ARRAYSIZE(RigidbodyType))) {
					vRigidbody2DComponent.m_Type = static_cast<Rigidbody2DComponent::RigidbodyType>(Index);
				}
				ImGui::Checkbox("Fixed Rotation", &vRigidbody2DComponent.m_IsFixedRotation);
				});

			drawEntityComponent<BoxCollider2DComponent>("BoxCollider2D Component", vEntity, [](auto& vBoxCollider2DComponent) {
				ImGui::DragFloat2("Offset", glm::value_ptr(vBoxCollider2DComponent.m_Offset), 0.1f, 0.0f, 100000.0f);
				ImGui::DragFloat2("Size", glm::value_ptr(vBoxCollider2DComponent.m_Size), 0.1f, 0.0f, 100000.0f);
				ImGui::DragFloat("Density", &vBoxCollider2DComponent.m_Density, 0.1f, 0.0f, 10.0f);
				ImGui::DragFloat("Friction", &vBoxCollider2DComponent.m_Friction, 0.01f, 0.0f, 1.0f);
				ImGui::DragFloat("Restitution", &vBoxCollider2DComponent.m_Restitution, 0.01f, 0.0f, 1.0f);
				ImGui::DragFloat("RestitutionThreshold", &vBoxCollider2DComponent.m_RestitutionThreshold, 0.01f, 0.0f, 1.0f);
				});
		}

		// Only 3D 
		if (m_Context->getSceneType() == Scene::SceneType::Scene3D) {
			drawEntityComponent<LightComponent>("Light", vEntity, [this](auto& vLightComponent) {
				auto& Light = vLightComponent.m_Light;
				std::string Type = Light ? Light->getType() : "None";
				ImGui::Columns(2);
				ImGui::SetColumnWidth(0, 100.0f);
				ImGui::Text("Type");
				ImGui::NextColumn();
				ImGui::Text(Type.c_str());
				ImGui::NextColumn();
				if (auto pDirectionLight = dynamic_cast<DirectionLight*>(vLightComponent.m_Light.get())) {
					auto& LightData = pDirectionLight->getData();
					ImGui::Text("Light Color");
					ImGui::NextColumn();
					ImGui::ColorEdit3("##Light Color", glm::value_ptr(LightData.m_LightColor));
					ImGui::NextColumn();
					ImGui::Text("Intensity");
					ImGui::NextColumn();
					ImGui::DragFloat("##Intensity", &LightData.m_Intensity, 0.05f, 0.0f, 100000.0f);
					ImGui::NextColumn();
					ImGui::Text("Shadow");
					ImGui::NextColumn();
					ImGui::Checkbox("##Direction Light Shadow", &LightData.m_EnableShadow);
					ImGui::NextColumn();
					ImGui::Text("Shadow Map");
					ImGui::NextColumn();
					uintptr_t ShadowMap = LightData.m_ShadowMap ? static_cast<uintptr_t>(LightData.m_ShadowMap->getRendererID())
						: static_cast<uintptr_t>(m_DefaultTexture->getRendererID());
					ImGui::ImageButton(reinterpret_cast<ImTextureID>(ShadowMap), { 50.0f, 50.0f }, { 0, 1 }, { 1, 0 });
					ImGui::NextColumn();
					ImGui::Text("Light Size");
					ImGui::NextColumn();
					ImGui::DragFloat("##Direction Light Size", &LightData.m_LightSize, 0.1f, 10.0f, 1000.0f);
					ImGui::NextColumn();
				}
				else if (auto pPointLight = dynamic_cast<PointLight*>(vLightComponent.m_Light.get())) {
					auto& LightData = pPointLight->getData();
					ImGui::Text("Light Color");
					ImGui::NextColumn();
					ImGui::ColorEdit3("##Light Color", glm::value_ptr(LightData.m_LightColor));
					ImGui::NextColumn();
					ImGui::Text("Intensity");
					ImGui::NextColumn();
					ImGui::DragFloat("##Intensity", &LightData.m_Intensity, 0.05f, 0.0f, 100000.0f);
					ImGui::NextColumn();
					ImGui::Text("Shadow");
					ImGui::NextColumn();
					ImGui::Checkbox("##Point Light Shadow", &LightData.m_EnableShadow);
					ImGui::NextColumn();
					ImGui::Text("Shadow Map");
					ImGui::NextColumn();
					uintptr_t ShadowMap = LightData.m_ShadowMap ? static_cast<uintptr_t>(LightData.m_ShadowMap->getRendererID())
						: static_cast<uintptr_t>(m_DefaultTexture->getRendererID());
					ImGui::ImageButton(reinterpret_cast<ImTextureID>(ShadowMap), { 50.0f, 50.0f }, { 0, 1 }, { 1, 0 });
					ImGui::NextColumn();
					ImGui::Text("Light Size");
					ImGui::NextColumn();
					ImGui::DragFloat("##Point Light Size", &LightData.m_LightSize, 0.1f, 5.0f, 100000.0f);
					ImGui::NextColumn();

				}
				ImGui::Columns();
				});
			drawEntityComponent<ModelComponent>("Model", vEntity, [](auto& vModelComponent) {
				std::string Path = vModelComponent.m_Model ? vModelComponent.m_Model->getPath().string() : "None";
				ImGui::Columns(2);
				ImGui::SetColumnWidth(0, 100.f);
				ImGui::Text("Path");
				ImGui::NextColumn();
				ImGui::Button(Path.c_str());
				if (ImGui::BeginDragDropTarget()) {
					if (const ImGuiPayload* Payload = ImGui::AcceptDragDropPayload("CONTENT_BROWSER_ITEM")) { // 对应ContentBrowserPanel中
						std::filesystem::path Path{ reinterpret_cast<const wchar_t*>(Payload->Data) };
						vModelComponent.m_Model = CreateRef<Model>(Path);
					}
					ImGui::EndDragDropTarget();
				}
				ImGui::Columns();
				});
			drawEntityComponent<MaterialComponent>("Material", vEntity, [this](auto& vMaterialComponent) {
				auto& Material = vMaterialComponent.m_Material;
				std::string Type = Material ? Material->getType() : "None";
				ImGui::Columns(2);
				ImGui::SetColumnWidth(0, 100.0f);
				ImGui::Text("Type");
				ImGui::NextColumn();
				ImGui::Text(Type.c_str());
				ImGui::NextColumn();
				if (auto pBlinnPhoneMaterial = dynamic_cast<BlinnPhoneMaterial*>(Material.get())) {
					auto& MaterialData = pBlinnPhoneMaterial->getData();
					ImGui::Text("Ambient");
					ImGui::NextColumn();
					ImGui::ColorEdit3("##Ambient", glm::value_ptr(MaterialData.m_Ambient));
					ImGui::NextColumn();
					ImGui::Text("Diffuse");
					ImGui::NextColumn();
					ImGui::ColorEdit3("##Diffuse", glm::value_ptr(MaterialData.m_Diffuse));
					ImGui::NextColumn();
					ImGui::Text("Specular");
					ImGui::NextColumn();
					ImGui::ColorEdit3("##Specular", glm::value_ptr(MaterialData.m_Specular));
					ImGui::NextColumn();
					ImGui::Text("Shininess");
					ImGui::NextColumn();
					ImGui::DragFloat("##Shininess", &MaterialData.m_Shininess, 0.05f, 0.0f, 100000.0f);
					ImGui::NextColumn();
					ImGui::Text("Diffuse Map");
					ImGui::NextColumn();
					uintptr_t DiffuseMap = MaterialData.m_DiffuseMap ? static_cast<uintptr_t>(MaterialData.m_DiffuseMap->getRendererID()) 
						: static_cast<uintptr_t>(m_DefaultTexture->getRendererID());
					ImGui::ImageButton(reinterpret_cast<ImTextureID>(DiffuseMap), { 50.0f, 50.0f }, { 0, 1 }, { 1, 0 });
					if (ImGui::BeginDragDropTarget()) {
						if (const ImGuiPayload* Payload = ImGui::AcceptDragDropPayload("CONTENT_BROWSER_ITEM")) { // 对应ContentBrowserPanel中
							std::filesystem::path Path{ reinterpret_cast<const wchar_t*>(Payload->Data) };
							const auto& Texture2D = Texture2D::Create(Path);
							MaterialData.m_DiffuseMap = Texture2D->getIsLoaded() ? Texture2D : nullptr;
						}
						ImGui::EndDragDropTarget();
					}
					if (ImGui::IsItemHovered() && ImGui::IsMouseReleased(ImGuiMouseButton_Right)) {
						ImGui::OpenPopup("DiffuseMap");
					}
					if (ImGui::BeginPopup("DiffuseMap")) {
						if (ImGui::MenuItem("Delete"))
							MaterialData.m_DiffuseMap = nullptr;
						ImGui::EndPopup();
					}
					ImGui::NextColumn();
					ImGui::Text("Specular Map");
					ImGui::NextColumn();
					uintptr_t SpecularMap = MaterialData.m_SpecularMap ? static_cast<uintptr_t>(MaterialData.m_SpecularMap->getRendererID()) 
						: static_cast<uintptr_t>(m_DefaultTexture->getRendererID());
					ImGui::ImageButton(reinterpret_cast<ImTextureID>(SpecularMap), { 50.0f, 50.0f }, { 0, 1 }, { 1, 0 });
					if (ImGui::BeginDragDropTarget()) {
						if (const ImGuiPayload* Payload = ImGui::AcceptDragDropPayload("CONTENT_BROWSER_ITEM")) { // 对应ContentBrowserPanel中
							std::filesystem::path Path{ reinterpret_cast<const wchar_t*>(Payload->Data) };
							const auto& Texture2D = Texture2D::Create(Path);
							MaterialData.m_SpecularMap = Texture2D->getIsLoaded() ? Texture2D : nullptr;
						}
						ImGui::EndDragDropTarget();
					}
					if (ImGui::IsItemHovered() && ImGui::IsMouseReleased(ImGuiMouseButton_Right)) {
						ImGui::OpenPopup("SpecularMap");
					}
					if (ImGui::BeginPopup("SpecularMap")) {
						if (ImGui::MenuItem("Delete"))
							MaterialData.m_SpecularMap = nullptr;
						ImGui::EndPopup();
					}
					ImGui::NextColumn();
				}
				// TODO : PBR Material
				ImGui::Columns();
				});
		}

	}

}