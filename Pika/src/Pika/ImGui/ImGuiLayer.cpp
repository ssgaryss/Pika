#include "pkpch.h"
#include "ImGuiLayer.h"

#include <imgui.h>
#include <GLFW/glfw3.h>
#include "Pika/Platform/OpenGL/imgui_impl_opengl3.h"
#include "Pika/Platform/OpenGL/imgui_impl_glfw.h"
#include "Pika/Core/Application.h"
#include "glad/glad.h"

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
		ImGui::CreateContext();
		ImGui::StyleColorsDark();

		ImGuiIO& io = ImGui::GetIO();
		io.BackendFlags |= ImGuiBackendFlags_HasMouseCursors;
		io.BackendFlags |= ImGuiBackendFlags_HasSetMousePos;

		ImGui_ImplOpenGL3_Init("#version 410");

	}
	void ImGuiLayer::onDetach()
	{
	}

	void ImGuiLayer::onUpdate()
	{
		ImGuiIO& io = ImGui::GetIO();
		Application& App{ Application::getInstance() };
		io.DisplaySize = ImVec2(static_cast<float>(App.getWindow().getWidth()), static_cast<float>(App.getWindow().getHeight()));

		float time = static_cast<float>(glfwGetTime());
		io.DeltaTime = m_Time > 0.0f ? (m_Time - time) : (1.0f, 60.0f);

		ImGui_ImplOpenGL3_NewFrame();
		ImGui::NewFrame();

		static bool Show = true;
		ImGui::ShowDemoWindow(&Show);
		ImGui::LogText("hello!");
		ImGui::Render();
		ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
	}

	void ImGuiLayer::onEvent(Event& vEvent)
	{
		EventDispatcher Dispatcher(vEvent);
		Dispatcher.dispatch<MouseButtonPressedEvent>(std::bind(&ImGuiLayer::onMouseButtonPressedEvent, this, std::placeholders::_1));
		Dispatcher.dispatch<MouseButtonRleasedEvent>(std::bind(&ImGuiLayer::onMouseButtonRleasedEvent, this, std::placeholders::_1));
		Dispatcher.dispatch<MouseMovedEvent>(std::bind(&ImGuiLayer::onMouseMovedEvent, this, std::placeholders::_1));
		Dispatcher.dispatch<MouseScrolledEvent>(std::bind(&ImGuiLayer::onMouseScrolledEvent, this, std::placeholders::_1));
		Dispatcher.dispatch<KeyPressedEvent>(std::bind(&ImGuiLayer::onKeyPressedEvent, this, std::placeholders::_1));
		Dispatcher.dispatch<KeyReleasedEvent>(std::bind(&ImGuiLayer::onKeyReleasedEvent, this, std::placeholders::_1));
		Dispatcher.dispatch<KeyTypedEvent>(std::bind(&ImGuiLayer::onKeyTypedEvent, this, std::placeholders::_1));
		Dispatcher.dispatch<WindowResizeEvent>(std::bind(&ImGuiLayer::onWindowResizeEvent, this, std::placeholders::_1));
	}

	bool ImGuiLayer::onMouseButtonPressedEvent(MouseButtonPressedEvent& vEvent)
	{
		ImGuiIO& io = ImGui::GetIO();
		io.MouseDown[vEvent.getMouseButton()] = true;
		PK_INFO("ImGui Layer handling event: {0}", vEvent.toString());
		return true;
	}

	bool ImGuiLayer::onMouseButtonRleasedEvent(MouseButtonRleasedEvent& vEvent)
	{
		ImGuiIO& io = ImGui::GetIO();
		io.MouseDown[vEvent.getMouseButton()] = false;
		PK_INFO("ImGui Layer handling event: {0}", vEvent.toString());
		return true;
	}

	bool ImGuiLayer::onMouseMovedEvent(MouseMovedEvent& vEvent)
	{
		ImGuiIO& io = ImGui::GetIO();
		io.MousePos = ImVec2(vEvent.getMouseX(), vEvent.getMouseY());
		PK_INFO("ImGui Layer handling event: {0}", vEvent.toString());
		return true;
	}

	bool ImGuiLayer::onMouseScrolledEvent(MouseScrolledEvent& vEvent)
	{
		ImGuiIO& io = ImGui::GetIO();
		io.MouseWheel = vEvent.getYOffet();
		io.MouseWheelH = vEvent.getXOffet();
		PK_INFO("ImGui Layer handling event: {0}", vEvent.toString());
		return true;
	}

	bool ImGuiLayer::onKeyPressedEvent(KeyPressedEvent& vEvent)
	{
		ImGuiIO& io = ImGui::GetIO();
		io.KeysDown[vEvent.getKeyCode()] = true;
		io.KeyCtrl = io.KeysDown[GLFW_KEY_LEFT_ALT] || io.KeysDown[GLFW_KEY_RIGHT_ALT];
		io.KeyAlt = io.KeysDown[GLFW_KEY_LEFT_CONTROL] || io.KeysDown[GLFW_KEY_RIGHT_CONTROL];
		io.KeyShift = io.KeysDown[GLFW_KEY_LEFT_SHIFT] || io.KeysDown[GLFW_KEY_RIGHT_SHIFT];
		io.KeySuper = io.KeysDown[GLFW_KEY_LEFT_SUPER] || io.KeysDown[GLFW_KEY_RIGHT_SUPER];
		PK_INFO("ImGui Layer handling event: {0}", vEvent.toString());
		return true;
	}

	bool ImGuiLayer::onKeyReleasedEvent(KeyReleasedEvent& vEvent)
	{
		ImGuiIO& io = ImGui::GetIO();
		io.KeysDown[vEvent.getKeyCode()] = false;
		PK_INFO("ImGui Layer handling event: {0}", vEvent.toString());
		return true;
	}

	bool ImGuiLayer::onKeyTypedEvent(KeyTypedEvent& vEvent)
	{
		ImGuiIO& io = ImGui::GetIO();
		int KeyCode = vEvent.getKeyCode();
		if (KeyCode > 0 && KeyCode < 0x10000) {
			io.AddInputCharacter(static_cast<unsigned int>(KeyCode));
		}
		PK_INFO("ImGui Layer handling event: {0}", vEvent.toString());
		return true;
	}

	bool ImGuiLayer::onWindowResizeEvent(WindowResizeEvent& vEvent)
	{
		ImGuiIO& io = ImGui::GetIO();
		io.DisplaySize = ImVec2(static_cast<float>(vEvent.getWidth()), static_cast<float>(vEvent.getHeight()));
		io.DisplayFramebufferScale = ImVec2(1.0f, 1.0f);
		glViewport(0, 0, vEvent.getWidth(), vEvent.getHeight());
		PK_INFO("ImGui Layer handling event: {0}", vEvent.toString());
		return true;
	}

}