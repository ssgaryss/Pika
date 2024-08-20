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
		m_LastFrameTime = vTimestep;

		// ����Scene��Scene������Cameras��ViewportSize
		m_Renderer->getContext()->onViewportResize(static_cast<uint32_t>(m_ViewportSize.x), static_cast<uint32_t>(m_ViewportSize.y));
		// ����FBO��EditorCamera��CameraController��Size��ʹ���С��Viewport����һ��
		if (const FramebufferSpecification& FS = m_Renderer->getFramebuffer()->getFramebufferSpecification();
			m_ViewportSize.x > 0.0f && m_ViewportSize.y > 0.0f
			&& (m_ViewportSize.x != FS.m_Width || m_ViewportSize.y != FS.m_Height))
		{
			m_Renderer->getFramebuffer()->resize(static_cast<uint32_t>(m_ViewportSize.x), static_cast<uint32_t>(m_ViewportSize.y));
			m_EditorCamera.setViewportSize(m_ViewportSize.x, m_ViewportSize.y);
		}

		m_Renderer->beginFrame();
		RenderCommand::SetClearColor({ 0.2f, 0.2f, 0.2f, 1.0f });
		RenderCommand::Clear(); // ��Ӱ������FBO�е�Texture����һ��
		m_Renderer->getFramebuffer()->clearAttachment(1, -1); // ����EntityID��������ֵ-1

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

		// ���������FBO��EntityID texture������
		auto MousePos = ImGui::GetMousePos(); // ��Ļ��������
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

		// Temporary Panels
		if (m_IsShowDemoPanel) ImGui::ShowDemoWindow(&m_IsShowDemoPanel);
		if (m_IsShowNewScenePanel) ShowNewScenePanel(&m_IsShowNewScenePanel);

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
				if (ImGui::MenuItem("Exit"))
					Application::GetInstance().close();
				ImGui::EndMenu();

			}

			if (ImGui::BeginMenu("Window")) {
				ImGui::Checkbox("Scene Hierarchy Panel##Window", m_SceneHierarchyPanel->getIsShowSceneHirarchy());
				ImGui::Checkbox("Content Browser Panel##Window", m_ContentBrowserPanel->getIsShowContentBrowser());
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

		// Statistics Panel
		ImGui::Begin("Renderer statistics");
		auto Statistics = Renderer2D::GetStatistics();
		std::string MouseHoveredEntityName = static_cast<bool>(m_MouseHoveredEntity) ? m_MouseHoveredEntity.getComponent<TagComponent>().m_Tag : "None";
		ImGui::Text("Mouse hovered entity : %s", MouseHoveredEntityName.c_str());
		ImGui::Text("DrawCalls : %d", Statistics.getDrawCalls());
		ImGui::Text("QuadCount : %d", Statistics.getQuadCount());
		ImGui::Text("LineCount : %d", Statistics.getLineCount());
		ImGui::Separator();
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
		ImGui::Text("Frame Time : %.2f ms", m_LastFrameTime.getMiliseconds());
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
				std::string PrimaryCameraName = "None";
				Entity PrimaryCamera = m_Renderer->getPrimaryCamera();
				if (PrimaryCamera && PrimaryCamera.hasComponent<CameraComponent>()) {
					m_Renderer->setPrimaryCamera(PrimaryCamera);
					PrimaryCameraName = PrimaryCamera.getComponent<TagComponent>().m_Tag;
				}
				ImGui::Button(PrimaryCameraName.c_str());
				if (ImGui::BeginDragDropTarget()) {
					if (const ImGuiPayload* Payload = ImGui::AcceptDragDropPayload("SCENE_CAMERA")) { // ��ӦContentBrowserPanel��
						const UUID& ID = *reinterpret_cast<const UUID*>(Payload->Data);
						m_Renderer->setPrimaryCamera(m_Renderer->getContext()->getEntityByUUID(ID));
					}
					ImGui::EndDragDropTarget();
				}
				ImGui::Separator();
				ImGui::Checkbox("Show Grid", m_Renderer->showGrid());
			}
		}
		ImGui::End();

		// Panels
		m_SceneHierarchyPanel->onImGuiRender();
		m_ContentBrowserPanel->onImGuiRender();
		m_SceneStatePanel->onImGuiRender();

		ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, { 0, 0 });
		ImGui::Begin("Viewport");
		// Viewport����¼���block����
		m_IsViewportFocus = ImGui::IsWindowFocused();
		m_IsViewportHovered = ImGui::IsWindowHovered();
		Application::GetInstance().getImGuiLayer()->setBlockEvents(!m_IsViewportFocus || !m_IsViewportHovered); // ImGuiLayerĬ���ǻ�Block�¼���

		// m_ViewportBounds
		auto WindowContentMinPoint = ImGui::GetWindowContentRegionMin();  // ImGui�������ϵ��Viewport��־���½ǲ���Content��ʼ��, ���������ǣ�0�� 0����
		auto WindowContentMaxPoint = ImGui::GetWindowContentRegionMax();  // ImGui�������ϵ��Viewport���½ǵ��������ϵ��
		auto ViewportOffset = ImGui::GetWindowPos(); // ��Ļ��������
		m_ViewportBounds[0] = { WindowContentMinPoint.x + ViewportOffset.x, WindowContentMinPoint.y + ViewportOffset.y }; // ���������Content��������� = Content����Ļ��������
		m_ViewportBounds[1] = { WindowContentMaxPoint.x + ViewportOffset.x, WindowContentMaxPoint.y + ViewportOffset.y }; // ���������Content��������� = Content����Ļ��������

		// m_ViewportSize 
		//ImGui::SetCursorPos(ImVec2{ 0, 0 }); // ��������ǿ����Panel�ģ�0��0��λ�ã���ʹ��Viewport��ǩ����
		ImVec2 ViewportPanelSize = ImGui::GetContentRegionAvail(); // ������Viewport��ǩ����,����ViewportPanel��С
		if (m_ViewportSize.x != ViewportPanelSize.x || m_ViewportSize.y != ViewportPanelSize.y)
			m_ViewportSize = { ViewportPanelSize.x, ViewportPanelSize.y };

		uintptr_t TextureID = static_cast<uintptr_t>(m_Renderer->getFramebuffer()->getColorAttachmentRendererID());
		ImGui::Image(reinterpret_cast<ImTextureID>(TextureID), { m_ViewportSize.x, m_ViewportSize.y }, { 0.0f,1.0f }, { 1.0f,0.0f });
		// ������קScenes
		if (ImGui::BeginDragDropTarget()) {
			if (const ImGuiPayload* Payload = ImGui::AcceptDragDropPayload("CONTENT_BROWSER_ITEM")) { // ��ӦContentBrowserPanel��
				std::filesystem::path Path = reinterpret_cast<const wchar_t*>(Payload->Data);
				if (Path.extension().string() == ".pika")
					openScene(Path);
				if (Path.extension().string() == ".png") {
					if (m_MouseHoveredEntity && m_MouseHoveredEntity.hasComponent<SpriteRendererComponent>())
						m_MouseHoveredEntity.getComponent<SpriteRendererComponent>().m_Texture = Texture2D::Create(Path);
				}
			}
			ImGui::EndDragDropTarget();
		}
		// Gizmos
		Entity SelectedEntity = m_SceneHierarchyPanel->getSelectedEntity();
		if (SelectedEntity && m_SceneStatePanel->getSceneState() == Scene::SceneState::Edit) {
			ImGuizmo::BeginFrame();
			ImGuizmo::SetDrawlist(); // ���û����б�draw list��,��ImGui�ṩ����ȾAPI
			ImGuizmo::SetOrthographic(m_EditorCamera.isOthograhic());
			ImGuizmo::SetRect(m_ViewportBounds[0].x, m_ViewportBounds[0].y,
				m_ViewportBounds[1].x - m_ViewportBounds[0].x,
				m_ViewportBounds[1].y - m_ViewportBounds[0].y); // ImGuizmo�Ļ�������

			auto& Transform = SelectedEntity.getComponent<TransformComponent>();
			glm::mat4 TransformMatrix = Transform;
			glm::vec3 Scale = Transform.m_Scale;
			glm::quat Orientation = glm::quat(Transform.m_Rotation);
			glm::vec3 Translation = Transform.m_Position;
			glm::vec3 Skew; // �б�
			glm::vec4 Perspective; // ͸�ӱ任
			if (m_GizmoType) {
				// TODO : �任Camera����
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
		m_Renderer->setContext(m_ActiveScene);
		m_Renderer->initialize();
		m_ActiveScenePath = std::filesystem::path(); // ����Ϊ��
	}

	void EditorLayer::openScene()
	{
		std::string Path = FileDialogs::OpenFile("Pika Scene\0*.pika\0"); // ����һ�飬ǰ����ʾΪ��ʾ���������ƣ�����Ϊʵ�ʺ�׺��
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
		// TODO : ��ʱֻ����.pika��׺Scene�ļ�
		if (vScenePath.extension().string() != ".pika") {
			PK_CORE_WARN("EditorLayer : Try to open a Pika scene with unknown extention.");
			return;
		}

		m_ActiveScene = CreateRef<Scene>();
		m_SceneHierarchyPanel->setContext(m_ActiveScene);
		m_SceneStatePanel->setContext(m_ActiveScene);
		auto Serializer = CreateRef<SceneSerializer>(m_ActiveScene);
		Serializer->deserializeYAMLText(Path);
		m_ActiveScenePath = std::filesystem::path(Path); // ����·��
		m_Renderer->setContext(m_ActiveScene);
		m_Renderer->initialize();   // Ҫ��deserialize֮��Ż�ȷ��SceneType

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

	void EditorLayer::ShowNewScenePanel(bool* vIsShow)
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

	void EditorLayer::initializeShortcutLibrary()
	{
		using namespace Key;
		// file�еĿ�ݼ�
		m_ShortcutLibrary.addShortcut({ "New_Scene", KeyCode::N, Shortcut::Modifier::Ctrl });
		m_ShortcutLibrary.addShortcut({ "Open_Scene", KeyCode::O, Shortcut::Modifier::Ctrl | Shortcut::Modifier::Shift });
		m_ShortcutLibrary.addShortcut({ "Save_Scene", KeyCode::S, Shortcut::Modifier::Ctrl });
		m_ShortcutLibrary.addShortcut({ "Save_Scene_As", KeyCode::S, Shortcut::Modifier::Ctrl | Shortcut::Modifier::Shift });
		// Gizmo״̬�л���
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
		// file�еĿ�ݼ�
		if (m_ShortcutLibrary["New_Scene"].IsHandleKeyEvent(vEvent))
			m_IsShowNewScenePanel = true;
		if (m_ShortcutLibrary["Open_Scene"].IsHandleKeyEvent(vEvent))
			openScene();
		if (m_ShortcutLibrary["Save_Scene"].IsHandleKeyEvent(vEvent))
			saveScene();
		if (m_ShortcutLibrary["Save_Scene_As"].IsHandleKeyEvent(vEvent))
			saveSceneAs();
		// Gizmo״̬�л���
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
		// EditorCamera��ݼ�
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
			ImVec2 MouseScreenPos = ImGui::GetMousePos(); // ��������
			ImVec2 MouseViewportPos = { MouseScreenPos.x - m_ViewportBounds[0].x, MouseScreenPos.y - m_ViewportBounds[0].y };
			if (m_IsViewportHovered && m_IsViewportFocus && (!m_SceneHierarchyPanel->getSelectedEntity() || !ImGuizmo::IsOver())) {
				if (!Input::isKeyPressed(Key::KeyCode::LeftAlt) && !Input::isKeyPressed(Key::KeyCode::RightAlt)) { // TODO : �����EditorCmara��ͻ�����ع�
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
