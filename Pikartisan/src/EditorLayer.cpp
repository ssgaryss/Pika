#include "EditorLayer.h"
#include <imgui/imgui.h>
#include <ImGuizmo/ImGuizmo.h>
#include "Pika/Utils/PlatformUtils.h"
#include <glm/gtc/type_ptr.hpp>
#include <glm/gtx/quaternion.hpp>
#include <glm/gtx/matrix_decompose.hpp>

namespace Pika
{

	// TODO : The tiled map example for now!
	static const char* s_Map =
		"000000000000000000000000000000"
		"000000000000000000000000000000"
		"000000000111111111111111110000"
		"000000111111111111111111100000"
		"000001111111100111111111000000"
		"000001111111000011111110000000"
		"000001111111000011111110000000"
		"000000111111111111111111000000"
		"000000001111111111111000000000"
		"000000000000000000000000000000"
		"000000000000000000000000000000"
		"000000000000000000000000000000"
		"000000000000000000000000000000"
		"000000011111111111111000000000"
		"000000111111111111111100000000"
		"000000000111111111100000000000"
		"000000011111111000000000000000"
		"000000000111000000111100000000"
		"000000000000000000011000000000"
		"000000000000000000000000000000";

	EditorLayer::EditorLayer()
		: Layer{ "EditorLayer" }
	{
	}

	void EditorLayer::onAttach()
	{
		PK_PROFILE_FUNCTION();
		// Initialize Renderer2D
		Renderer2D::Initialize();
		// Initialize Scene
		m_ActiveScene = CreateRef<Scene>();
		// Initialize Scene Renderer
		m_Renderer = CreateRef<SceneRenderer>();
		m_Renderer->setContext(m_ActiveScene);
		m_Renderer->setFramebuffer(Framebuffer::Create({ 1920, 1080, 1,
			{TextureFormat::RGBA8, TextureFormat::R16I, TextureFormat::DEPTH24STENCIL8}, false }));
		m_Renderer->initialize();
		// Initialize Shortcuts
		initializeShortcutLibrary();
		// Initialize Panels
		m_SceneHierarchyPanel = CreateScope<SceneHierarchyPanel>(m_ActiveScene);
		m_ContentBrowserPanel = CreateScope<ContentBrowserPanel>();
		m_SceneStatePanel = CreateScope<SceneStatePanel>(m_ActiveScene);

		// TODO : Delete!!!
		m_TextureBackround = Texture2D::Create("assets/textures/board.png");
		m_Texture2024 = Texture2D::Create("assets/textures/2024.png");
		m_TextureRPGpack_sheet_2X = Texture2D::Create("assets/textures/RPGpack_sheet_2X.png");

		m_TextureTree = SubTexture2D::Create(m_TextureRPGpack_sheet_2X, { 2, 1 }, { 1, 2 }, { 128, 128 });
		m_TextureWater = SubTexture2D::Create(m_TextureRPGpack_sheet_2X, { 11, 11 }, { 1, 1 }, { 128, 128 });
		m_TextureGround = SubTexture2D::Create(m_TextureRPGpack_sheet_2X, { 1, 11 }, { 1, 1 }, { 128, 128 });
	}

	void EditorLayer::onDetach()
	{
		PK_PROFILE_FUNCTION();
	}

	void EditorLayer::onUpdate(Timestep vTimestep)
	{
		PK_PROFILE_FUNCTION();

		// 更新Scene和Scene中所有Cameras的ViewportSize
		m_ActiveScene->onViewportResize(static_cast<uint32_t>(m_ViewportSize.x), static_cast<uint32_t>(m_ViewportSize.y));
		// 更新FBO、EditorCamera和CameraController的Size，使其大小与Viewport保持一致
		if (const FramebufferSpecification& FS = m_Renderer->getFramebuffer()->getFramebufferSpecification();
			m_ViewportSize.x > 0.0f && m_ViewportSize.y > 0.0f
			&& (m_ViewportSize.x != FS.m_Width || m_ViewportSize.y != FS.m_Height))
		{
			m_Renderer->getFramebuffer()->resize(static_cast<uint32_t>(m_ViewportSize.x), static_cast<uint32_t>(m_ViewportSize.y));
			m_EditorCamera.setViewportSize(m_ViewportSize.x, m_ViewportSize.y);
		}

		m_Renderer->beginFrame();
		RenderCommand::SetClearColor({ 0.2f, 0.2f, 0.2f, 1.0f });
		RenderCommand::Clear(); // 会影响所有FBO中的Texture而非一个
		m_Renderer->getFramebuffer()->clearAttachment(1, -1); // 所有EntityID其余区域赋值-1

#if 0
		Renderer2D::BeginScene(m_CameraController);
		Renderer2D::DrawQuad({ 0.0f, 0.0f, -0.9f }, { 20.0f, 20.0f }, m_TextureBackround, 10.0f);
		Renderer2D::DrawQuad({ 0.5f, 0.5f }, { 0.5f, 0.5f }, { 1.0f, 0.0f, 1.0f, 1.0f });
		Renderer2D::DrawQuad({ -0.5f, 0.5f }, { 0.5f, 0.5f }, { 0.0f, 0.0f, 1.0f, 1.0f });
		Renderer2D::DrawQuad({ 0.5f, -0.5f }, { 0.2f, 0.5f }, { 0.5f, 0.5f, 1.0f, 1.0f });
		Renderer2D::DrawRotatedQuad({ -0.5f, -0.5f }, { 0.5f, 0.5f }, glm::radians(Rotation), { 1.0f, 0.0f, 1.0f, 1.0f });
		Renderer2D::DrawRotatedQuad({ -0.5f, -0.5f, 0.1f }, { 0.5f, 0.5f }, glm::radians(Rotation), m_Texture2024);
		int num = 0;
		for (int i = 0; i < 20; ++i) {
			for (int j = 0; j < 30; ++j) {
				if (s_Map[i * 30 + j] == '0')
					Renderer2D::DrawQuad({ -3.0f + j * 0.2f, 2.0f - i * 0.2f }, { 0.2f, 0.2f }, m_TextureWater);
				else if (s_Map[i * 30 + j] == '1')
					Renderer2D::DrawQuad({ -3.0f + j * 0.2f, 2.0f - i * 0.2f }, { 0.2f, 0.2f }, m_TextureGround);
				else
					Renderer2D::DrawQuad({ -3.0f + j * 0.2f, 2.0f - i * 0.2f }, { 0.2f, 0.2f }, m_TextureTree);
			}
		}

		Rotation += glm::radians(10.0f);
		Renderer2D::EndScene();

		//Renderer2D::BeginScene(m_CameraController);
		//m_ActiveScene->onUpdate(vTimestep);
		//Renderer2D::EndScene();
#endif // 0

		m_ActiveScene->onUpdate(vTimestep);
		switch (m_SceneStatePanel->getSceneState())
		{
		case Pika::Scene::SceneState::Edit:
		{
			if (m_IsViewportFocus)
				m_EditorCamera.onUpdate(vTimestep);
			m_Renderer->render(m_EditorCamera);
			break;
		}
		case Pika::Scene::SceneState::Play:
		{
			break;
		}
		case Pika::Scene::SceneState::Simulate:
		{
			// TODO!
			break;
		}
		}

		// 计算鼠标在FBO中EntityID texture的坐标
		auto MousePos = ImGui::GetMousePos(); // 屏幕绝对坐标
		int EntityIDTextureX = static_cast<int>(MousePos.x - m_ViewportBounds[0].x);
		int EntityIDTextureY = static_cast<int>(m_ViewportBounds[1].y - MousePos.y);
		if (EntityIDTextureX >= 0 && EntityIDTextureX < (int)m_ViewportSize.x && EntityIDTextureY >= 0 && EntityIDTextureY < (int)m_ViewportSize.y) {
			int EntityID = m_Renderer->getFramebuffer()->readPixel(1, EntityIDTextureX, EntityIDTextureY);
			m_MouseHoveredEntity = EntityID == -1 ? Entity{} : Entity{ static_cast<entt::entity>(EntityID), m_ActiveScene.get() };
		}

		m_Renderer->endFrame();
	}

	void EditorLayer::onImGuiRender()
	{
		PK_PROFILE_FUNCTION();

		static bool dockspace_open = true;
		static bool opt_fullscreen = true;
		static bool opt_padding = false;
		static ImGuiDockNodeFlags dockspace_flags = ImGuiDockNodeFlags_None;

		// We are using the ImGuiWindowFlags_NoDocking flag to make the parent window not dockable into,
		// because it would be confusing to have two docking targets within each others.
		ImGuiWindowFlags window_flags = ImGuiWindowFlags_MenuBar | ImGuiWindowFlags_NoDocking;
		if (opt_fullscreen)
		{
			const ImGuiViewport* viewport = ImGui::GetMainViewport();
			ImGui::SetNextWindowPos(viewport->WorkPos);
			ImGui::SetNextWindowSize(viewport->WorkSize);
			ImGui::SetNextWindowViewport(viewport->ID);
			ImGui::PushStyleVar(ImGuiStyleVar_WindowRounding, 0.0f);
			ImGui::PushStyleVar(ImGuiStyleVar_WindowBorderSize, 0.0f);
			window_flags |= ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoMove;
			window_flags |= ImGuiWindowFlags_NoBringToFrontOnFocus | ImGuiWindowFlags_NoNavFocus;
		}
		else
		{
			dockspace_flags &= ~ImGuiDockNodeFlags_PassthruCentralNode;
		}

		if (dockspace_flags & ImGuiDockNodeFlags_PassthruCentralNode)
			window_flags |= ImGuiWindowFlags_NoBackground;

		if (!opt_padding)
			ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(0.0f, 0.0f));

		ImGui::Begin("DockSpace Demo", &dockspace_open, window_flags);
		if (!opt_padding)
			ImGui::PopStyleVar();

		if (opt_fullscreen)
			ImGui::PopStyleVar(2);

		// Submit the DockSpace
		ImGuiIO& IO = ImGui::GetIO();
		if (IO.ConfigFlags & ImGuiConfigFlags_DockingEnable)
		{
			ImGuiID dockspace_id = ImGui::GetID("MyDockSpace");
			ImGui::DockSpace(dockspace_id, ImVec2(0.0f, 0.0f), dockspace_flags);
		}
		else
		{
		}

		if (ImGui::BeginMenuBar())
		{
			if (ImGui::BeginMenu("Files"))
			{
				// File Menu
				if (ImGui::MenuItem("New", m_ShortcutLibrary["New_Scene"].toString().c_str()))
					newScene();
				if (ImGui::MenuItem("Open ...", m_ShortcutLibrary["Open_Scene"].toString().c_str()))
					openScene();
				if (ImGui::MenuItem("Save", m_ShortcutLibrary["Save_Scene"].toString().c_str()))
					saveScene();
				if (ImGui::MenuItem("Save as ...", m_ShortcutLibrary["Save_Scene_As"].toString().c_str()))
					saveSceneAs();
				if (ImGui::MenuItem("Exit"))
					Application::GetInstance().close();
				ImGui::EndMenu();
			}

			if (ImGui::BeginMenu("Window")) {
				ImGui::Checkbox("Scene Hierarchy Panel##Window", m_SceneHierarchyPanel->getIsShowSceneHirarchy());
				ImGui::Checkbox("Content Browser Panel##Window", m_ContentBrowserPanel->getIsShowContentBrowser());
				ImGui::EndMenu();
			}

			//if (ImGui::BeginMenu("ImGui Settings")) {
			//	// Disabling fullscreen would allow the window to be moved to the front of other windows,
			//	// which we can't undo at the moment without finer window depth/z control.
			//	ImGui::MenuItem("Fullscreen", NULL, &opt_fullscreen);
			//	ImGui::MenuItem("Padding", NULL, &opt_padding);
			//	ImGui::Separator();

			//	if (ImGui::MenuItem("Flag: NoDockingOverCentralNode", "", (dockspace_flags & ImGuiDockNodeFlags_NoDockingOverCentralNode) != 0)) { dockspace_flags ^= ImGuiDockNodeFlags_NoDockingOverCentralNode; }
			//	if (ImGui::MenuItem("Flag: NoDockingSplit", "", (dockspace_flags & ImGuiDockNodeFlags_NoDockingSplit) != 0)) { dockspace_flags ^= ImGuiDockNodeFlags_NoDockingSplit; }
			//	if (ImGui::MenuItem("Flag: NoUndocking", "", (dockspace_flags & ImGuiDockNodeFlags_NoUndocking) != 0)) { dockspace_flags ^= ImGuiDockNodeFlags_NoUndocking; }
			//	if (ImGui::MenuItem("Flag: NoResize", "", (dockspace_flags & ImGuiDockNodeFlags_NoResize) != 0)) { dockspace_flags ^= ImGuiDockNodeFlags_NoResize; }
			//	if (ImGui::MenuItem("Flag: AutoHideTabBar", "", (dockspace_flags & ImGuiDockNodeFlags_AutoHideTabBar) != 0)) { dockspace_flags ^= ImGuiDockNodeFlags_AutoHideTabBar; }
			//	if (ImGui::MenuItem("Flag: PassthruCentralNode", "", (dockspace_flags & ImGuiDockNodeFlags_PassthruCentralNode) != 0, opt_fullscreen)) { dockspace_flags ^= ImGuiDockNodeFlags_PassthruCentralNode; }
			//	ImGui::Separator();

			//	if (ImGui::MenuItem("Close", NULL, false, dockspace_open != NULL))
			//		dockspace_open = false;
			//	ImGui::EndMenu();
			//}

			ImGui::EndMenuBar();
		}

		// Statistics Panel
		ImGui::Begin("Renderer statistics");
		auto Statistics = Renderer2D::GetStatistics();
		std::string MouseHoveredEntityName = static_cast<bool>(m_MouseHoveredEntity) ? m_MouseHoveredEntity.getComponent<TagComponent>().m_Tag : "None";
		ImGui::Text("Mouse hovered entity : %s", MouseHoveredEntityName.c_str());
		ImGui::Text("DrawCalls : %d", Statistics.getDrawCalls());
		ImGui::Text("QuadCount : %d", Statistics.getQuadCount());
		ImGui::Separator();
		uintptr_t DepthID = static_cast<uintptr_t>(m_Renderer->getFramebuffer()->getDepthStencilAttachmentRendererID());
		ImGui::Image(reinterpret_cast<ImTextureID>(DepthID), { 300.0f, 300.0f * (m_ViewportSize.y / m_ViewportSize.x) }, { 0.0f,1.0f }, { 1.0f,0.0f });
		ImGui::Separator();
		ImGui::End(); // Renderer statistics

		// Scene Renderer Panel
		ImGui::Begin("Scene Renderer Settings");
		if (m_ActiveScene) {
			auto& Context = m_Renderer->getContext();
			if (Context) {
				ImGui::Text("Scene Name : ");
				ImGui::SameLine();
				static char Buffer[256];
				memset(Buffer, 0, sizeof(Buffer));
				strcpy_s(Buffer, Context->getSceneName().c_str());
				if (ImGui::InputText("##SceneName", Buffer, sizeof(Buffer)))
					Context->setSceneName(std::string(Buffer));
				ImGui::Text("Scene Type : ");
				ImGui::SameLine();
				const char* SceneTypeName[] = { "2D Scene", "3D Scene" };
				ImGui::Text(SceneTypeName[static_cast<int>(Context->getSceneType())]);
				const char* SceneModeName[] = { "Edit Mode", "Play Mode", "Simulate Mode" };
				ImGui::Text("Scene State : ");
				ImGui::SameLine();
				ImGui::Text(SceneModeName[static_cast<int>(m_SceneStatePanel->getSceneState())]);
				ImGui::Text("Primary Camera : ");
				ImGui::SameLine();
				static std::vector<std::string> Cameras = { "Item 1", "Item 2", "Item 3" };
				static int currentItem = 0;
				// Combo 开始
				if (ImGui::BeginCombo("##PrimaryCamera", Cameras[currentItem].c_str()))
				{
					for (int i = 0; i < Cameras.size(); ++i)
					{
						bool isSelected = (currentItem == i);
						if (ImGui::Selectable(Cameras[i].c_str(), isSelected))
						{
							currentItem = i;
						}
						if (isSelected)
						{
							ImGui::SetItemDefaultFocus();
						}
					}
					ImGui::EndCombo();
				}
			}
		}
		ImGui::End();

		// Panels
		m_SceneHierarchyPanel->onImGuiRender();
		m_ContentBrowserPanel->onImGuiRender();
		m_SceneStatePanel->onImGuiRender();

		ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, { 0, 0 });
		ImGui::Begin("Viewport");
		// Viewport鼠标事件的block条件
		m_IsViewportFocus = ImGui::IsWindowFocused();
		m_IsViewportHovered = ImGui::IsWindowHovered();
		Application::GetInstance().getImGuiLayer()->setBlockEvents(!m_IsViewportFocus || !m_IsViewportHovered); // ImGuiLayer默认是会Block事件的

		// m_ViewportBounds
		auto WindowContentMinPoint = ImGui::GetWindowContentRegionMin();  // ImGui相对坐标系（Viewport标志左下角才是Content开始点, 若隐藏则是（0， 0））
		auto WindowContentMaxPoint = ImGui::GetWindowContentRegionMax();  // ImGui相对坐标系（Viewport右下角的相对坐标系）
		auto ViewportOffset = ImGui::GetWindowPos(); // 屏幕绝对坐标
		m_ViewportBounds[0] = { WindowContentMinPoint.x + ViewportOffset.x, WindowContentMinPoint.y + ViewportOffset.y }; // 绝对坐标加Content的相对坐标 = Content的屏幕绝对坐标
		m_ViewportBounds[1] = { WindowContentMaxPoint.x + ViewportOffset.x, WindowContentMaxPoint.y + ViewportOffset.y }; // 绝对坐标加Content的相对坐标 = Content的屏幕绝对坐标

		// m_ViewportSize 
		//ImGui::SetCursorPos(ImVec2{ 0, 0 }); // 这会让组件强制在Panel的（0，0）位置，即使被Viewport标签覆盖
		ImVec2 ViewportPanelSize = ImGui::GetContentRegionAvail(); // 包含了Viewport标签部分,整个ViewportPanel大小
		if (m_ViewportSize.x != ViewportPanelSize.x || m_ViewportSize.y != ViewportPanelSize.y)
			m_ViewportSize = { ViewportPanelSize.x, ViewportPanelSize.y };

		uintptr_t TextureID = static_cast<uintptr_t>(m_Renderer->getFramebuffer()->getColorAttachmentRendererID());
		ImGui::Image(reinterpret_cast<ImTextureID>(TextureID), { m_ViewportSize.x, m_ViewportSize.y }, { 0.0f,1.0f }, { 1.0f,0.0f });
		// 接收拖拽Scenes
		if (ImGui::BeginDragDropTarget()) {
			if (const ImGuiPayload* Payload = ImGui::AcceptDragDropPayload("CONTENT_BROWSER_ITEM")) { // 对应ContentBrowserPanel中
				const wchar_t* Path = reinterpret_cast<const wchar_t*>(Payload->Data);
				openScene(Path);
			}
			ImGui::EndDragDropTarget();
		}
		// Gizmos
		Entity SelectedEntity = m_SceneHierarchyPanel->getSelectedEntity();
		if (SelectedEntity) {
			ImGuizmo::BeginFrame();
			ImGuizmo::SetDrawlist(); // 设置绘制列表（draw list）,即ImGui提供的渲染API
			ImGuizmo::SetOrthographic(m_EditorCamera.isOthograhic()); // TODO! : CameraComponent情况
			ImGuizmo::SetRect(m_ViewportBounds[0].x, m_ViewportBounds[0].y,
				m_ViewportBounds[1].x - m_ViewportBounds[0].x,
				m_ViewportBounds[1].y - m_ViewportBounds[0].y); // ImGuizmo的绘制区域

			auto& Transform = SelectedEntity.getComponent<TransformComponent>();
			glm::mat4 TransformMatrix = Transform;
			glm::vec3 Scale = Transform.m_Scale;
			glm::quat Orientation = glm::quat(Transform.m_Rotation);
			glm::vec3 Translation = Transform.m_Position;
			glm::vec3 Skew; // 切变
			glm::vec4 Perspective; // 透视变换
			if (m_GizmoType) {
				// TODO : 变换Camera参数
				ImGuizmo::Manipulate(glm::value_ptr(m_EditorCamera.getViewMatrix()),
					glm::value_ptr(m_EditorCamera.getProjectionMatrix()),
					static_cast<ImGuizmo::OPERATION>(m_GizmoType), ImGuizmo::LOCAL, glm::value_ptr(TransformMatrix));
			}

			if (ImGuizmo::IsUsing()) {
				// TODO : Math decompose function
				bool Success = glm::decompose(TransformMatrix, Scale, Orientation, Translation, Skew, Perspective);
				PK_ASSERT(Success, "Fail to decompose TransformMatrix!");
				if (Success) {
					Transform.m_Position = Translation;
					Transform.m_Rotation = glm::degrees(glm::eulerAngles(Orientation));
					Transform.m_Scale = Scale;
				}
			}
		}

		ImGui::End(); // Viewport
		ImGui::PopStyleVar();

		ImGui::End(); // DockSpace Demo
	}

	void EditorLayer::onEvent(Event& vEvent)
	{
		PK_PROFILE_FUNCTION();
		//m_CameraController.onEvent(vEvent);
		m_EditorCamera.onEvent(vEvent);
		EventDispatcher Dispatcher(vEvent);
		Dispatcher.dispatch<KeyPressedEvent>(std::bind_front(&EditorLayer::onKeyPressed, this));
		Dispatcher.dispatch<MouseButtonPressedEvent>(std::bind_front(&EditorLayer::onMousePressed, this));
	}

	void EditorLayer::newScene()
	{
		m_ActiveScene = CreateRef<Scene>();
		m_SceneHierarchyPanel->setContext(m_ActiveScene);
		m_ActiveScenePath = std::filesystem::path(); // 重置为空
	}

	void EditorLayer::openScene()
	{
		std::string Path = FileDialogs::OpenFile("Pika Scene\0*.pika\0"); // 两两一组，前面显示为显示过滤器名称，后面为实际后缀名
		if (Path.empty()) {
			PK_CORE_WARN("EditorLayer : Try to open a Pika scene with null path.");
			return;
		}

		std::filesystem::path ScenePath(Path);
		openScene(ScenePath);
	}

	void EditorLayer::openScene(const std::filesystem::path& vScenePath)
	{
		std::string Path = vScenePath.string();
		if (Path.empty()) {
			PK_CORE_WARN("EditorLayer : Try to open a Pika scene with null path.");
			return;
		}
		// TODO : 暂时只允许.pika后缀Scene文件
		if (vScenePath.extension().string() != ".pika") {
			PK_CORE_WARN("EditorLayer : Try to open a Pika scene with unknown extention.");
			return;
		}

		m_ActiveScene = CreateRef<Scene>();
		m_SceneHierarchyPanel->setContext(m_ActiveScene);
		m_SceneStatePanel->setContext(m_ActiveScene);
		m_Renderer->setContext(m_ActiveScene);
		auto Serializer = CreateRef<SceneSerializer>(m_ActiveScene);
		Serializer->deserializeYAMLText(Path);
		m_ActiveScenePath = std::filesystem::path(Path); // 绝对路径

	}

	void EditorLayer::saveScene()
	{
		std::string	Path = m_ActiveScenePath.string();

		if (!Path.empty()) {
			auto Serializer = CreateRef<SceneSerializer>(m_ActiveScene);
			Serializer->serializeYAMLText(Path);
		}
		else {
			saveSceneAs();
		}
	}

	void EditorLayer::saveSceneAs()
	{
		std::string Path = FileDialogs::SaveFile("Pika Scene\0*.pika\0");
		if (Path.empty()) {
			PK_CORE_WARN("EditorLayer : Try to save a Pika scene with null path.");
			return;
		}

		auto Serializer = CreateRef<SceneSerializer>(m_ActiveScene);
		Serializer->serializeYAMLText(Path);
	}

	void EditorLayer::initializeShortcutLibrary()
	{
		using namespace Key;
		// file中的快捷键
		m_ShortcutLibrary.addShortcut({ "New_Scene", KeyCode::N, Shortcut::Modifier::Ctrl });
		m_ShortcutLibrary.addShortcut({ "Open_Scene", KeyCode::O, Shortcut::Modifier::Ctrl | Shortcut::Modifier::Shift });
		m_ShortcutLibrary.addShortcut({ "Save_Scene", KeyCode::S, Shortcut::Modifier::Ctrl });
		m_ShortcutLibrary.addShortcut({ "Save_Scene_As", KeyCode::S, Shortcut::Modifier::Ctrl | Shortcut::Modifier::Shift });
		// Gizmo状态切换键
		m_ShortcutLibrary.addShortcut({ "Gizmo_None", KeyCode::Q });
		m_ShortcutLibrary.addShortcut({ "Gizmo_Translate", KeyCode::W });
		m_ShortcutLibrary.addShortcut({ "Gizmo_Rotate", KeyCode::E });
		m_ShortcutLibrary.addShortcut({ "Gizmo_Scale", KeyCode::R });
		// EditorCamera Action
		m_ShortcutLibrary.addShortcut({ "Focus_Entity", KeyCode::F });
	}

	bool EditorLayer::onKeyPressed(KeyPressedEvent& vEvent)
	{
		using namespace Key;
		// file中的快捷键
		if (m_ShortcutLibrary["New_Scene"].IsHandleKeyEvent(vEvent))
			newScene();
		if (m_ShortcutLibrary["Open_Scene"].IsHandleKeyEvent(vEvent))
			openScene();
		if (m_ShortcutLibrary["Save_Scene"].IsHandleKeyEvent(vEvent))
			saveScene();
		if (m_ShortcutLibrary["Save_Scene_As"].IsHandleKeyEvent(vEvent))
			saveSceneAs();
		// Gizmo状态切换键
		if (m_ShortcutLibrary["Gizmo_None"].IsHandleKeyEvent(vEvent)) {
			if (!ImGuizmo::IsUsing())
				m_GizmoType = 0;
		}
		if (m_ShortcutLibrary["Gizmo_Translate"].IsHandleKeyEvent(vEvent)) {
			if (!ImGuizmo::IsUsing())
				m_GizmoType = ImGuizmo::OPERATION::TRANSLATE;
		}
		if (m_ShortcutLibrary["Gizmo_Rotate"].IsHandleKeyEvent(vEvent)) {
			if (!ImGuizmo::IsUsing())
				m_GizmoType = ImGuizmo::OPERATION::ROTATE;
		}
		if (m_ShortcutLibrary["Gizmo_Scale"].IsHandleKeyEvent(vEvent)) {
			if (!ImGuizmo::IsUsing())
				m_GizmoType = ImGuizmo::OPERATION::SCALE;
		}
		// EditorCamera快捷键
		if (m_ShortcutLibrary["Focus_Entity"].IsHandleKeyEvent(vEvent)) {
			auto Entity = m_SceneHierarchyPanel->getSelectedEntity();
			m_EditorCamera.setFocalPoint(Entity.getComponent<TransformComponent>().m_Position);
		}
		return false;
	}

	bool EditorLayer::onMousePressed(MouseButtonPressedEvent& vEvent)
	{
		using namespace Mouse;
		switch (vEvent.getMouseButton())
		{
		case MouseCode::ButtonLeft:
		{
			ImVec2 MouseScreenPos = ImGui::GetMousePos(); // 绝对坐标
			ImVec2 MouseViewportPos = { MouseScreenPos.x - m_ViewportBounds[0].x, MouseScreenPos.y - m_ViewportBounds[0].y };
			if (m_IsViewportHovered && m_IsViewportFocus && (!m_SceneHierarchyPanel->getSelectedEntity() || !ImGuizmo::IsOver())) {
				if (!Input::isKeyPressed(Key::KeyCode::LeftAlt) && !Input::isKeyPressed(Key::KeyCode::RightAlt)) { // TODO : 避免和EditorCmara冲突，须重构
					m_SceneHierarchyPanel->setSelectedEntity(m_MouseHoveredEntity);
					m_GizmoType = m_GizmoType == 0 ? ImGuizmo::OPERATION::TRANSLATE : m_GizmoType;
				}
			}

			break;
		}
		default:
			break;
		}
		return false;
	}

}
