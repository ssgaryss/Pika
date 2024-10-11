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

		// Initialize Scene
		m_ActiveScene = CreateRef<Scene>();
		// Initialize Scene Renderer
		m_Renderer = CreateRef<SceneRenderer>();
		m_Renderer->setRenderDataExtractor(CreateRef<RenderDataExtractor>(m_ActiveScene));
		m_Renderer->setFramebuffer(Framebuffer::Create({ 1920, 1080, 1,
			{TextureFormat::RGBA8, TextureFormat::R32I, TextureFormat::DEPTH24STENCIL8}, false })); // 这里EntityID一定用R32I，因为R16I会发生阶段导致bug！！！
		m_Renderer->initialize();

		// Initialize Shortcuts
		initializeShortcutLibrary();
		// Initialize Panels
		m_SceneHierarchyPanel = CreateScope<SceneHierarchyPanel>(m_ActiveScene);
		m_ContentBrowserPanel = CreateScope<ContentBrowserPanel>();
		m_SceneStatePanel = CreateScope<SceneStatePanel>(m_ActiveScene);
		// Initialize Default Skybox
		m_ActiveScene->setSkybox(Cubemap::Create("resources/skybox/kloofendal_48d_partly_cloudy_puresky_2k.png"));
	}

	void EditorLayer::onDetach()
	{
		PK_PROFILE_FUNCTION();
	}

	void EditorLayer::onUpdate(Timestep vTimestep)
	{
		PK_PROFILE_FUNCTION();
		m_LastFrameTime = vTimestep;

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

		m_ActiveScene->onUpdate(vTimestep);

		switch (m_SceneStatePanel->getSceneState())
		{
		case Pika::Scene::SceneState::Edit:
		{
			if (m_IsViewportFocus && m_IsViewportHovered)
				m_EditorCamera.onUpdate(vTimestep);
			m_Renderer->render(m_EditorCamera);
			break;
		}
		case Pika::Scene::SceneState::Play:
		{
			m_Renderer->render();
			break;
		}
		case Pika::Scene::SceneState::Simulate:
		{
			if (m_IsViewportFocus)
				m_EditorCamera.onUpdate(vTimestep);
			m_Renderer->render(m_EditorCamera);
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

	}

	void EditorLayer::onImGuiRender()
	{
		PK_PROFILE_FUNCTION();

		// Temporary Panels
		if (m_IsShowDemoPanel) ImGui::ShowDemoWindow(&m_IsShowDemoPanel);
		if (m_IsShowNewScenePanel) showNewScenePanel(&m_IsShowNewScenePanel);

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
				if (ImGui::MenuItem("New Scene", m_ShortcutLibrary["New_Scene"].toString().c_str()))
					m_IsShowNewScenePanel = true;
				if (ImGui::MenuItem("Open Scene...", m_ShortcutLibrary["Open_Scene"].toString().c_str()))
					openScene();
				if (ImGui::MenuItem("Save Scene", m_ShortcutLibrary["Save_Scene"].toString().c_str()))
					saveScene();
				if (ImGui::MenuItem("Save Scene as ...", m_ShortcutLibrary["Save_Scene_As"].toString().c_str()))
					saveSceneAs();
				ImGui::Separator();
				if (ImGui::MenuItem("Exit"))
					Application::GetInstance().close();
				ImGui::EndMenu();

			}

			if (ImGui::BeginMenu("Window")) {
				ImGui::Checkbox("Scene Hierarchy Panel##Window", m_SceneHierarchyPanel->getIsShowSceneHirarchy());
				ImGui::Checkbox("Content Browser Panel##Window", m_ContentBrowserPanel->getIsShowContentBrowser());
				ImGui::Checkbox("Renderer Statistics Panel##Window", &m_IsShowRendererStatistics);
				ImGui::Checkbox("Scene Renderer Settings Panel##Window", &m_IsShowSceneRendererSettings);
				ImGui::Checkbox("Show Demo Panel##Window", &m_IsShowDemoPanel);
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

		// Panels
		if (m_IsShowRendererStatistics) showRendererStatistics(&m_IsShowRendererStatistics);
		if (m_IsShowSceneRendererSettings) showSceneRendererSettings(&m_IsShowSceneRendererSettings);
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
				std::filesystem::path Path = reinterpret_cast<const wchar_t*>(Payload->Data);
				if (Path.extension().string() == ".pika")
					openScene(Path);
				if (Path.extension().string() == ".png") {
					if (m_MouseHoveredEntity && m_MouseHoveredEntity.hasComponent<SpriteRendererComponent>()) {
						const auto& Texture2D = Texture2D::Create(Path);
						m_MouseHoveredEntity.getComponent<SpriteRendererComponent>().m_Texture = Texture2D->getIsLoaded() ? Texture2D : nullptr;
					}
				}
			}
			ImGui::EndDragDropTarget();
		}
		// Gizmos
		Entity SelectedEntity = m_SceneHierarchyPanel->getSelectedEntity();
		if (SelectedEntity && m_SceneStatePanel->getSceneState() == Scene::SceneState::Edit && SelectedEntity.hasComponent<TransformComponent>()) {
			ImGuizmo::BeginFrame();
			ImGuizmo::SetDrawlist(); // 设置绘制列表（draw list）,即ImGui提供的渲染API
			ImGuizmo::SetOrthographic(m_EditorCamera.isOthograhic());
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

	void EditorLayer::newScene(const std::string& vName, Scene::SceneType vType)
	{
		m_ActiveScene = CreateRef<Scene>(vName, vType);
		m_SceneHierarchyPanel->setContext(m_ActiveScene);
		m_SceneStatePanel->setContext(m_ActiveScene);
		m_Renderer->setRenderDataExtractor(CreateRef<RenderDataExtractor>(m_ActiveScene));
		m_Renderer->initialize();
		resetEditorState();
		m_ActiveScenePath = std::filesystem::path(); // 重置为空
	}

	void EditorLayer::resetEditorState()
	{
		// 重置一些和场景相关的变量
		m_MouseHoveredEntity = {};
		m_GizmoType = 0;
		bool m_IsViewportFocus = false;
		bool m_IsViewportHovered = false;
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
		auto Serializer = CreateRef<SceneSerializer>(m_ActiveScene);
		Serializer->deserializeYAMLText(Path);
		m_ActiveScenePath = std::filesystem::path(Path); // 绝对路径
		m_Renderer->setRenderDataExtractor(CreateRef<RenderDataExtractor>(m_ActiveScene));
		m_Renderer->initialize();   // 要在deserialize之后才会确定SceneType

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

	void EditorLayer::showNewScenePanel(bool* vIsShow)
	{
		ImVec2 Center = ImGui::GetMainViewport()->GetCenter();
		ImGui::SetNextWindowPos(Center, ImGuiCond_Appearing, { 0.5f, 0.5f });
		ImGui::SetNextWindowSize(ImVec2(520, 600), ImGuiCond_FirstUseEver);
		static ImVec2 Padding{ 80.0f, 30.0f };
		ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, Padding);
		if (!ImGui::Begin("Scene Settings", vIsShow))
		{
			ImGui::End();
			return;
		}
		ImGui::Columns(2, nullptr, false);
		ImGui::SetColumnWidth(0, 100.0f);
		ImGui::Text("Scene Name : ");
		ImGui::NextColumn();
		static char Buffer[256] = "Untitled";
		ImGui::InputText("##SceneType", Buffer, sizeof(Buffer));

		ImGui::NextColumn();
		ImGui::Text("Scene Type : ");
		ImGui::NextColumn();
		static std::string CurrentSceneType = "Scene 2D";
		if (ImGui::BeginCombo("##SelectSceneType", CurrentSceneType.c_str())) {
			if (ImGui::Selectable("Scene2D", CurrentSceneType == "Scene 2D"))
				CurrentSceneType = "Scene 2D";
			if (ImGui::Selectable("Scene3D", CurrentSceneType == "Scene 3D"))
				CurrentSceneType = "Scene 3D";
			ImGui::EndCombo();
		}
		ImGui::Columns();

		static ImVec2 ButtonSize{ 70.0f, 0.0f };
		ImGui::SetCursorPos({ ImGui::GetContentRegionAvail().x / 2.0f - ButtonSize.x + Padding.x, ImGui::GetCursorPosY() + 30.0f });
		if (ImGui::Button("Confirm", ButtonSize)) {
			auto SceneType = CurrentSceneType == "Scene 2D" ? Scene::SceneType::Scene2D : Scene::SceneType::Scene3D;
			newScene(std::string(Buffer), SceneType);
			*vIsShow = false;
		}
		ImGui::SameLine();
		if (ImGui::Button("Cancel", ButtonSize))
			*vIsShow = false;
		ImGui::End();
		ImGui::PopStyleVar();
	}

	void EditorLayer::showRendererStatistics(bool* vIsShow)
	{
		ImGui::Begin("Renderer statistics");
		const auto ContextInformation = reinterpret_cast<const GraphicsContext::GraphicsContextInformation*>(Application::GetInstance().getWindow().getContextInformation());
		ImGui::Columns(2, nullptr, false);
		ImGui::SetColumnWidth(0, 80.0f);
		ImGui::Text("Vendor : ", ContextInformation->m_Vendor.c_str());
		ImGui::NextColumn();
		ImGui::Text(ContextInformation->m_Vendor.c_str());
		ImGui::NextColumn();
		ImGui::Text("Renderer : ");
		ImGui::NextColumn();
		ImGui::Text(ContextInformation->m_Renderer.c_str());
		ImGui::NextColumn();
		ImGui::Text("Version : ");
		ImGui::NextColumn();
		ImGui::Text(ContextInformation->m_Version.c_str());
		ImGui::Columns();
		ImGui::Separator();
		std::string MouseHoveredEntityName = static_cast<bool>(m_MouseHoveredEntity) && m_MouseHoveredEntity.hasComponent<TagComponent>()
			? m_MouseHoveredEntity.getComponent<TagComponent>().m_Tag : "None";
		ImGui::Text("Mouse hovered entity : %s", MouseHoveredEntityName.c_str());
		if (m_ActiveScene->getSceneType() == Scene::SceneType::Scene2D) {
			auto Statistics = Renderer2D::GetStatistics();
			ImGui::Text("DrawCalls : %d", Statistics.getDrawCalls());
			ImGui::Text("QuadCount : %d", Statistics.getQuadCount());
			ImGui::Text("LineCount : %d", Statistics.getLineCount());
		}
		else {
			auto Statistics = Renderer3D::GetStatistics();
			ImGui::Text("DrawCalls : %d", Statistics.getDrawCalls());
			ImGui::Text("MeshCount : %d", Statistics.getMeshCount());
			ImGui::Text("LineCount : %d", Statistics.getLineCount());
		}
		ImGui::Text("Frame Time : %.2f ms", m_LastFrameTime.getMiliseconds());
		ImGui::Separator();
		ImGui::Text("Depth map :");
		uintptr_t DepthID = static_cast<uintptr_t>(m_Renderer->getFramebuffer()->getDepthStencilAttachmentRendererID());
		ImGui::Image(reinterpret_cast<ImTextureID>(DepthID), { 300.0f, 300.0f * (m_ViewportSize.y / m_ViewportSize.x) }, { 0.0f,1.0f }, { 1.0f,0.0f });
		ImGui::Separator();
		ImGui::End(); // Renderer statistics

	}

	void EditorLayer::showSceneRendererSettings(bool* vIsShow)
	{
		ImGui::Begin("Scene Renderer Settings");
		if (m_ActiveScene) {
			auto& Context = m_ActiveScene;
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
				ImGui::Separator();
				ImGui::Text("Primary Camera : ");
				ImGui::SameLine();
				std::string PrimaryCameraName = "None";
				Entity PrimaryCamera = m_Renderer->getPrimaryCamera();
				if (PrimaryCamera && PrimaryCamera.hasComponent<CameraComponent>())
					PrimaryCameraName = PrimaryCamera.getComponent<TagComponent>().m_Tag;
				ImGui::Button(PrimaryCameraName.c_str());
				if (ImGui::BeginDragDropTarget()) {
					if (const ImGuiPayload* Payload = ImGui::AcceptDragDropPayload("SCENE_CAMERA")) { // 对应SceneHierarchyPanel中
						const UUID& ID = *reinterpret_cast<const UUID*>(Payload->Data);
						Entity CameraEntity = m_ActiveScene->getEntityByUUID(ID);
						if (CameraEntity.hasComponent<CameraComponent>())
							m_Renderer->setPrimaryCamera(CameraEntity);
						else
							PK_WARN("Fail to set primary camera with an entity without camera component.");
					}
					ImGui::EndDragDropTarget();
				}
				ImGui::Text("Skybox");
				ImGui::SameLine();
				const auto& Skybox = m_ActiveScene->getSkybox();
				std::string SkyboxName = Skybox ? Skybox->getPath().filename().string() : "None";
				ImGui::Button(SkyboxName.c_str());
				if (ImGui::BeginDragDropTarget()) {
					if (const ImGuiPayload* Payload = ImGui::AcceptDragDropPayload("CONTENT_BROWSER_ITEM")) { // 对应ContentBrowserPanel中
						std::filesystem::path Path = reinterpret_cast<const wchar_t*>(Payload->Data);
						if (Path.extension().string() == ".png" || Path.extension().string() == ".hdr") {
							m_ActiveScene->setSkybox(Cubemap::Create(Path));
						}
						//TODO : HDR
					}
					ImGui::EndDragDropTarget();
				}
				ImGui::Separator();
				ImGui::Checkbox("Show Grid", m_Renderer->showGrid());
			}
		}
		ImGui::End();
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
			m_IsShowNewScenePanel = true;
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
