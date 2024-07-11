#include "EditorLayer.h"
#include <imgui/imgui.h>
#include <glm/gtc/type_ptr.hpp>

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
		: Layer{ "EditorLayer" }, m_CameraController{ 1920.0f / 1080.0f },
		m_ShaderLibrary{ CreateRef<ShaderLibrary>() }
	{
	}

	void EditorLayer::onAttach()
	{
		PK_PROFILE_FUNCTION();
		Renderer2D::Init();
		m_Framebuffer = Framebuffer::Create({ 1920, 1080, 1,
			{TextureFormat::RGB8, TextureFormat::RGB8, TextureFormat::DEPTH24STENCIL8}, false });
		m_ActiveScene = CreateRef<Scene>();
		m_SceneSerilizer = CreateRef<SceneSerializer>(m_ActiveScene);
		m_BulueQuad = m_ActiveScene->createEntity("Blue quad");
		m_BulueQuad.addComponent<SpriteRendererComponent>(glm::vec4(0.1f, 0.1f, 1.0f, 1.0f));

		m_RedQuad = m_ActiveScene->createEntity("Red quad");
		m_RedQuad.addComponent<SpriteRendererComponent>(glm::vec4(1.0f, 0.1f, 0.1f, 1.0f));

		m_TextureBackround = Texture2D::Create("assets/textures/board.png");
		m_Texture2024 = Texture2D::Create("assets/textures/2024.png");
		m_TextureRPGpack_sheet_2X = Texture2D::Create("assets/textures/RPGpack_sheet_2X.png");

		m_TextureTree = SubTexture2D::Create(m_TextureRPGpack_sheet_2X, { 2, 1 }, { 1, 2 }, { 128, 128 });
		m_TextureWater = SubTexture2D::Create(m_TextureRPGpack_sheet_2X, { 11, 11 }, { 1, 1 }, { 128, 128 });
		m_TextureGround = SubTexture2D::Create(m_TextureRPGpack_sheet_2X, { 1, 11 }, { 1, 1 }, { 128, 128 });

		// Panels
		m_SceneHierarchyPanel = CreateScope<SceneHierarchyPanel>(m_ActiveScene);
	}

	void EditorLayer::onDetach()
	{
		PK_PROFILE_FUNCTION();
	}

	void EditorLayer::onUpdate(Timestep vTimestep)
	{
		PK_PROFILE_FUNCTION();

		{
			PK_PROFILE_SCOPE("Renderer Prep");
			m_Framebuffer->bind();
			RenderCommand::SetClearColor({ 0.2f, 0.2f, 0.2f, 1.0f });
			RenderCommand::Clear();
		}

		if (m_IsViewportFocus)
			m_CameraController.onUpdate(vTimestep);

		//Renderer2D::BeginScene(m_CameraController);
		//Renderer2D::DrawQuad({ 0.0f, 0.0f, -0.9f }, { 20.0f, 20.0f }, m_TextureBackround, 10.0f);
		//Renderer2D::DrawQuad({ 0.5f, 0.5f }, { 0.5f, 0.5f }, { 1.0f, 0.0f, 1.0f, 1.0f });
		//Renderer2D::DrawQuad({ -0.5f, 0.5f }, { 0.5f, 0.5f }, { 0.0f, 0.0f, 1.0f, 1.0f });
		//Renderer2D::DrawQuad({ 0.5f, -0.5f }, { 0.2f, 0.5f }, { 0.5f, 0.5f, 1.0f, 1.0f });
		//Renderer2D::DrawRotatedQuad({ -0.5f, -0.5f }, { 0.5f, 0.5f }, glm::radians(Rotation), { 1.0f, 0.0f, 1.0f, 1.0f });
		//Renderer2D::DrawRotatedQuad({ -0.5f, -0.5f, 0.1f }, { 0.5f, 0.5f }, glm::radians(Rotation), m_Texture2024);
		//int num = 0;
		//for (int i = 0; i < 20; ++i) {
		//	for (int j = 0; j < 30; ++j) {
		//		if (s_Map[i * 30 + j] == '0')
		//			Renderer2D::DrawQuad({ -3.0f + j * 0.2f, 2.0f - i * 0.2f }, { 0.2f, 0.2f }, m_TextureWater);
		//		else if (s_Map[i * 30 + j] == '1')
		//			Renderer2D::DrawQuad({ -3.0f + j * 0.2f, 2.0f - i * 0.2f }, { 0.2f, 0.2f }, m_TextureGround);
		//		else
		//			Renderer2D::DrawQuad({ -3.0f + j * 0.2f, 2.0f - i * 0.2f }, { 0.2f, 0.2f }, m_TextureTree);
		//	}
		//}

		//Rotation += glm::radians(10.0f);
		//Renderer2D::EndScene();

		Renderer2D::BeginScene(m_CameraController);
		m_ActiveScene->onUpdate(vTimestep);
		Renderer2D::EndScene();

		m_Framebuffer->unbind();

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
				// Disabling fullscreen would allow the window to be moved to the front of other windows,
				// which we can't undo at the moment without finer window depth/z control.
				ImGui::MenuItem("Fullscreen", NULL, &opt_fullscreen);
				ImGui::MenuItem("Padding", NULL, &opt_padding);
				ImGui::Separator();

				if (ImGui::MenuItem("Flag: NoDockingOverCentralNode", "", (dockspace_flags & ImGuiDockNodeFlags_NoDockingOverCentralNode) != 0)) { dockspace_flags ^= ImGuiDockNodeFlags_NoDockingOverCentralNode; }
				if (ImGui::MenuItem("Flag: NoDockingSplit", "", (dockspace_flags & ImGuiDockNodeFlags_NoDockingSplit) != 0)) { dockspace_flags ^= ImGuiDockNodeFlags_NoDockingSplit; }
				if (ImGui::MenuItem("Flag: NoUndocking", "", (dockspace_flags & ImGuiDockNodeFlags_NoUndocking) != 0)) { dockspace_flags ^= ImGuiDockNodeFlags_NoUndocking; }
				if (ImGui::MenuItem("Flag: NoResize", "", (dockspace_flags & ImGuiDockNodeFlags_NoResize) != 0)) { dockspace_flags ^= ImGuiDockNodeFlags_NoResize; }
				if (ImGui::MenuItem("Flag: AutoHideTabBar", "", (dockspace_flags & ImGuiDockNodeFlags_AutoHideTabBar) != 0)) { dockspace_flags ^= ImGuiDockNodeFlags_AutoHideTabBar; }
				if (ImGui::MenuItem("Flag: PassthruCentralNode", "", (dockspace_flags & ImGuiDockNodeFlags_PassthruCentralNode) != 0, opt_fullscreen)) { dockspace_flags ^= ImGuiDockNodeFlags_PassthruCentralNode; }
				ImGui::Separator();

				if (ImGui::MenuItem("Close", NULL, false, dockspace_open != NULL))
					dockspace_open = false;
				if (ImGui::MenuItem("Exit")) Application::GetInstance().close();
				ImGui::EndMenu();
			}

			ImGui::EndMenuBar();
		}

		auto Statistics = Renderer2D::GetStatistics();
		ImGui::Begin("Renderer statistics");
		ImGui::Text("DrawCalls : %d", Statistics.getDrawCalls());
		ImGui::Text("QuadCount : %d", Statistics.getQuadCount());
		ImGui::Separator();
		uintptr_t DepthID = static_cast<uintptr_t>(m_Framebuffer->getDepthStencilAttachmentRendererID());
		ImGui::Image(reinterpret_cast<void*>(DepthID), { 384.0f, 256.0f }, { 0.0f,1.0f }, { 1.0f,0.0f });
		ImGui::Separator();
		uintptr_t ColorID = static_cast<uintptr_t>(m_Framebuffer->getColorAttachmentRendererID(1));
		ImGui::Image(reinterpret_cast<void*>(ColorID), { 384.0f, 256.0f }, { 0.0f,1.0f }, { 1.0f,0.0f });
		ImGui::Separator();
		ImGui::End();

		// SceneHierarchyPanel
		m_SceneHierarchyPanel->onImGuiRender();

		ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, { 0, 0 });
		ImGui::Begin("Viewport");
		{
			m_IsViewportFocus = ImGui::IsWindowFocused();
			m_IsViewportHovered = ImGui::IsWindowHovered();
			Application::GetInstance().getImGuiLayer()->setBlockEvents(!m_IsViewportFocus || !m_IsViewportHovered);
		}
		{
			static glm::vec2 ViewportSize;
			ImVec2 ViewportPanelSize = ImGui::GetContentRegionAvail();
			if (ViewportPanelSize.x != ViewportPanelSize.x || ViewportPanelSize.y != ViewportPanelSize.y) {
				m_Framebuffer->resize((uint32_t)ViewportPanelSize.x, (uint32_t)ViewportPanelSize.y);
				ViewportSize = { ViewportPanelSize.x, ViewportPanelSize.y };
			}
			m_CameraController.onResize(ViewportPanelSize.x, ViewportPanelSize.y);
			uintptr_t TextureID = static_cast<uintptr_t>(m_Framebuffer->getColorAttachmentRendererID());
			ImGui::Image(reinterpret_cast<void*>(TextureID), { ViewportPanelSize.x, ViewportPanelSize.y }, { 0.0f,1.0f }, { 1.0f,0.0f });
		}
		ImGui::End();
		ImGui::PopStyleVar();

		ImGui::End();
	}

	void EditorLayer::onEvent(Event& vEvent)
	{
		PK_PROFILE_FUNCTION();
		m_CameraController.onEvent(vEvent);
	}

}
