#include "EditorLayer.h"
#include <imgui/imgui.h>

namespace Pika
{

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
		m_ShaderLibrary{ Pika::CreateRef<Pika::ShaderLibrary>() }
	{
	}

	void EditorLayer::onAttach()
	{
		PK_PROFILE_FUNCTION();
		Pika::Renderer2D::Init();
		m_Framebuffer = Pika::Framebuffer::Create({ 1920,1080,1,
			{FramebufferTextureFormat::RGB8, FramebufferTextureFormat::RGB8, FramebufferTextureFormat::Depth},false });

		m_TextureBackround = Pika::Texture2D::Create("assets/textures/board.png");
		m_Texture2024 = Pika::Texture2D::Create("assets/textures/2024.png");
		m_TextureRPGpack_sheet_2X = Pika::Texture2D::Create("assets/textures/RPGpack_sheet_2X.png");

		m_TextureTree = Pika::SubTexture2D::Create(m_TextureRPGpack_sheet_2X, { 2, 1 }, { 1, 2 }, { 128, 128 });
		m_TextureWater = Pika::SubTexture2D::Create(m_TextureRPGpack_sheet_2X, { 11, 11 }, { 1, 1 }, { 128, 128 });
		m_TextureGround = Pika::SubTexture2D::Create(m_TextureRPGpack_sheet_2X, { 1, 11 }, { 1, 1 }, { 128, 128 });
	}

	void EditorLayer::onDetach()
	{
		PK_PROFILE_FUNCTION();
	}

	void EditorLayer::onUpdate(Pika::Timestep vTimestep)
	{
		PK_PROFILE_FUNCTION();

		{
			PK_PROFILE_SCOPE("Renderer Prep");
			m_Framebuffer->bind();
			RenderCommand::SetClearColor({ 0.2f, 0.2f, 0.2f, 1.0f });
			RenderCommand::Clear();
		}

		m_CameraController.onUpdate(vTimestep);
		Pika::Renderer2D::BeginScene(m_CameraController);
		Pika::Renderer2D::DrawQuad({ 0.0f, 0.0f, -0.9f }, { 20.0f, 20.0f }, m_TextureBackround, 10.0f);
		Pika::Renderer2D::DrawQuad({ 0.5f, 0.5f }, { 0.5f, 0.5f }, { 1.0f, 0.0f, 1.0f, 1.0f });
		Pika::Renderer2D::DrawQuad({ -0.5f, 0.5f }, { 0.5f, 0.5f }, { 0.0f, 0.0f, 1.0f, 1.0f });
		Pika::Renderer2D::DrawQuad({ 0.5f, -0.5f }, { 0.2f, 0.5f }, { 0.5f, 0.5f, 1.0f, 1.0f });
		Pika::Renderer2D::DrawRotatedQuad({ -0.5f, -0.5f }, { 0.5f, 0.5f }, glm::radians(Rotation), { 1.0f, 0.0f, 1.0f, 1.0f });
		Pika::Renderer2D::DrawRotatedQuad({ -0.5f, -0.5f, 0.1f }, { 0.5f, 0.5f }, glm::radians(Rotation), m_Texture2024);
		int num = 0;
		for (int i = 0; i < 20; ++i) {
			for (int j = 0; j < 30; ++j) {
				if (s_Map[i * 30 + j] == '0')
					Pika::Renderer2D::DrawQuad({ -3.0f + j * 0.2f, 2.0f - i * 0.2f }, { 0.2f, 0.2f }, m_TextureWater);
				else if (s_Map[i * 30 + j] == '1')
					Pika::Renderer2D::DrawQuad({ -3.0f + j * 0.2f, 2.0f - i * 0.2f }, { 0.2f, 0.2f }, m_TextureGround);
				else
					Pika::Renderer2D::DrawQuad({ -3.0f + j * 0.2f, 2.0f - i * 0.2f }, { 0.2f, 0.2f }, m_TextureTree);
			}
		}

		Rotation += glm::radians(10.0f);
		Pika::Renderer2D::EndScene();
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

		// When using ImGuiDockNodeFlags_PassthruCentralNode, DockSpace() will render our background
		// and handle the pass-thru hole, so we ask Begin() to not render a background.
		if (dockspace_flags & ImGuiDockNodeFlags_PassthruCentralNode)
			window_flags |= ImGuiWindowFlags_NoBackground;

		// Important: note that we proceed even if Begin() returns false (aka window is collapsed).
		// This is because we want to keep our DockSpace() active. If a DockSpace() is inactive,
		// all active windows docked into it will lose their parent and become undocked.
		// We cannot preserve the docking relationship between an active window and an inactive docking, otherwise
		// any change of dockspace/settings would lead to windows being stuck in limbo and never being visible.
		if (!opt_padding)
			ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(0.0f, 0.0f));
		ImGui::Begin("DockSpace Demo", &dockspace_open, window_flags);
		if (!opt_padding)
			ImGui::PopStyleVar();

		if (opt_fullscreen)
			ImGui::PopStyleVar(2);

		// Submit the DockSpace
		ImGuiIO& io = ImGui::GetIO();
		if (io.ConfigFlags & ImGuiConfigFlags_DockingEnable)
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
				if (ImGui::MenuItem("Exit")) Pika::Application::GetInstance().close();
				ImGui::EndMenu();
			}

			ImGui::EndMenuBar();
		}

		auto Statistics = Pika::Renderer2D::GetStatistics();
		ImGui::Begin("Renderer statistics");
		ImGui::Text("DrawCalls : %d", Statistics.getDrawCalls());
		ImGui::Text("QuadCount : %d", Statistics.getQuadCount());
		ImGui::End();

		ImGui::Begin("Viewport");
		ImVec2 ViewportPanelSize = ImGui::GetContentRegionAvail();
		//PK_INFO("WIDTH :{}, HEIGHT :{}", ViewportPanelSize.x, ViewportPanelSize.y);
		uintptr_t TextureID = static_cast<uintptr_t>(m_Framebuffer->getColorAttachmentRendererID());
		ImGui::Image(reinterpret_cast<void*>(TextureID), { 1280.0f,720.0f }, { 0.0f,1.0f }, { 1.0f,0.0f });
		ImGui::End();


		ImGui::End();
	}

	void EditorLayer::onEvent(Pika::Event& vEvent)
	{
		PK_PROFILE_FUNCTION();
		m_CameraController.onEvent(vEvent);
	}

}
