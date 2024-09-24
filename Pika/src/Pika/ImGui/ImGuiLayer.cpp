#include "pkpch.h"
#include "ImGuiLayer.h"

#include <imgui.h>
#include <GLFW/glfw3.h>
#include <backends/imgui_impl_opengl3.h>
#include <backends/imgui_impl_glfw.h>
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include "Pika/Core/Application.h"


namespace Pika {
	ImGuiLayer::ImGuiLayer(const std::string& vLayerName) :
		Layer{ vLayerName }
	{
	}
	ImGuiLayer::~ImGuiLayer()
	{
	}
	void ImGuiLayer::onAttach()
	{
		PK_PROFILE_FUNCTION();

		IMGUI_CHECKVERSION();
		ImGui::CreateContext();

		ImGuiIO& IO = ImGui::GetIO();
		IO.ConfigFlags |= ImGuiConfigFlags_DockingEnable;
		//io.ConfigFlags |= ImGuiConfigFlags_NavEnableGamepad;
		IO.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;
		IO.ConfigFlags |= ImGuiConfigFlags_ViewportsEnable;

		// font
		IO.FontDefault = IO.Fonts->AddFontFromFileTTF("resources/fonts/Open_Sans/static/OpenSans_Condensed-Bold.ttf", 18.0f);
		IO.FontDefault = IO.Fonts->AddFontFromFileTTF("resources/fonts/Open_Sans/static/OpenSans-Regular.ttf", 18.0f);

		//UI Theme
		setDarkThemeColors();
		//ImGui::StyleColorsDark(); //自带风格
		//ImGui::StyleColorsClassic(); //自带风格

		Application& App = Application::GetInstance();
		GLFWwindow* Window = reinterpret_cast<GLFWwindow*>(App.getWindow().getNativeWindow());

		ImGui_ImplGlfw_InitForOpenGL(Window, true);
		ImGui_ImplOpenGL3_Init("#version 460");

	}
	void ImGuiLayer::onDetach()
	{
		PK_PROFILE_FUNCTION();

		ImGui_ImplOpenGL3_Shutdown();
		ImGui_ImplGlfw_Shutdown();
		ImGui::DestroyContext();
	}

	void ImGuiLayer::onEvent(Event& vEvent)
	{
		PK_PROFILE_FUNCTION();

		if (m_BlockEvents) {
			ImGuiIO& IO = ImGui::GetIO();
			//ImGui Layer can handle mouse and keyboard events
			vEvent.m_Handled |= vEvent.isInCategory(EventCategoryMouse) & IO.WantCaptureMouse;
			vEvent.m_Handled |= vEvent.isInCategory(EventCategoryKeyboard) & IO.WantCaptureKeyboard;
		}
	}

	void ImGuiLayer::begin()
	{
		PK_PROFILE_FUNCTION();

		ImGui_ImplOpenGL3_NewFrame(); //update GL context status
		ImGui_ImplGlfw_NewFrame(); //update GLFW context status
		ImGui::NewFrame();
	}

	void ImGuiLayer::end()
	{
		PK_PROFILE_FUNCTION();

		ImGuiIO& IO = ImGui::GetIO();
		Application& App = Application::GetInstance();
		IO.DisplaySize = ImVec2(static_cast<float>(App.getWindow().getWidth()),
								static_cast<float>(App.getWindow().getHeight()));

		ImGui::Render();
		ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());

		if (IO.ConfigFlags & ImGuiConfigFlags_ViewportsEnable) {
			GLFWwindow* Backup = glfwGetCurrentContext();
			ImGui::UpdatePlatformWindows();
			ImGui::RenderPlatformWindowsDefault();
			glfwMakeContextCurrent(Backup);
		}
	}

	void ImGuiLayer::setDarkThemeColors()
	{
		auto& colors = ImGui::GetStyle().Colors;
		colors[ImGuiCol_WindowBg] = ImVec4{ 0.1f, 0.105f, 0.11f, 1.0f };

		// Headers
		colors[ImGuiCol_Header] = ImVec4{ 0.2f, 0.205f, 0.21f, 1.0f };
		colors[ImGuiCol_HeaderHovered] = ImVec4{ 0.3f, 0.305f, 0.31f, 1.0f };
		colors[ImGuiCol_HeaderActive] = ImVec4{ 0.15f, 0.1505f, 0.151f, 1.0f };

		// Buttons
		colors[ImGuiCol_Button] = ImVec4{ 0.2f, 0.205f, 0.21f, 1.0f };
		colors[ImGuiCol_ButtonHovered] = ImVec4{ 0.3f, 0.305f, 0.31f, 1.0f };
		colors[ImGuiCol_ButtonActive] = ImVec4{ 0.15f, 0.1505f, 0.151f, 1.0f };

		// Frame BG
		colors[ImGuiCol_FrameBg] = ImVec4{ 0.2f, 0.205f, 0.21f, 1.0f };
		colors[ImGuiCol_FrameBgHovered] = ImVec4{ 0.3f, 0.305f, 0.31f, 1.0f };
		colors[ImGuiCol_FrameBgActive] = ImVec4{ 0.15f, 0.1505f, 0.151f, 1.0f };

		// Tabs
		colors[ImGuiCol_Tab] = ImVec4{ 0.15f, 0.1505f, 0.151f, 1.0f };
		colors[ImGuiCol_TabHovered] = ImVec4{ 0.38f, 0.3805f, 0.381f, 1.0f };
		colors[ImGuiCol_TabActive] = ImVec4{ 0.28f, 0.2805f, 0.281f, 1.0f };
		colors[ImGuiCol_TabUnfocused] = ImVec4{ 0.15f, 0.1505f, 0.151f, 1.0f };
		colors[ImGuiCol_TabUnfocusedActive] = ImVec4{ 0.2f, 0.205f, 0.21f, 1.0f };

		// Title
		colors[ImGuiCol_TitleBg] = ImVec4{ 0.15f, 0.1505f, 0.151f, 1.0f };
		colors[ImGuiCol_TitleBgActive] = ImVec4{ 0.15f, 0.1505f, 0.151f, 1.0f };
		colors[ImGuiCol_TitleBgCollapsed] = ImVec4{ 0.15f, 0.1505f, 0.151f, 1.0f };
	}

}