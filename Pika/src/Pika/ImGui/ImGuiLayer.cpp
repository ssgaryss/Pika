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
		IMGUI_CHECKVERSION();
		ImGui::CreateContext();

		ImGuiIO& io = ImGui::GetIO();
		io.ConfigFlags |= ImGuiConfigFlags_DockingEnable;
		//io.ConfigFlags |= ImGuiConfigFlags_NavEnableGamepad;
		io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;
		io.ConfigFlags |= ImGuiConfigFlags_ViewportsEnable;

		ImGui::StyleColorsDark();
		//ImGui::StyleColorsClassic();
		Application& App = Application::getInstance();
		GLFWwindow* Window = reinterpret_cast<GLFWwindow*>(App.getWindow().getNativeWindow());

		ImGui_ImplGlfw_InitForOpenGL(Window, true);
		ImGui_ImplOpenGL3_Init("#version 410");

	}
	void ImGuiLayer::onDetach()
	{
	}

	void ImGuiLayer::onEvent(Event& vEvent)
	{
		if (m_BlockEvents) {
			ImGuiIO& io = ImGui::GetIO();
			//ImGui Layer can handle mouse and keyboard events
			vEvent.m_Handled = vEvent.isInCategory(EventCategoryMouse) && io.WantCaptureMouse;
			vEvent.m_Handled = vEvent.isInCategory(EventCategoryKeyboard) && io.WantCaptureKeyboard;
		}
	}

	void ImGuiLayer::begin()
	{
		ImGui_ImplOpenGL3_NewFrame(); //update GL context status
		ImGui_ImplGlfw_NewFrame(); //update GLFW context status
		ImGui::NewFrame();
	}

	void ImGuiLayer::end()
	{
		ImGuiIO& io = ImGui::GetIO();
		Application& App = Application::getInstance();
		io.DisplaySize = ImVec2(static_cast<float>(App.getWindow().getWidth()),
								static_cast<float>(App.getWindow().getHeight()));

		ImGui::Render();
		ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());

		if (io.ConfigFlags & ImGuiConfigFlags_ViewportsEnable) {
			GLFWwindow* Backup = glfwGetCurrentContext();
			ImGui::UpdatePlatformWindows();
			ImGui::RenderPlatformWindowsDefault();
			glfwMakeContextCurrent(Backup);
		}
	}


}