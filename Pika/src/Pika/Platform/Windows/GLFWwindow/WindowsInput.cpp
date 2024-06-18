#include "pkpch.h"
#include "Pika/Core/Input.h"
#include "Pika/Core/Application.h"
#include <GLFW/glfw3.h>

namespace Pika {

	bool Input::isKeyPressed(const Key::KeyCode vKey) {
		auto* Window = reinterpret_cast<GLFWwindow*>(Application::GetInstance().getWindow().getNativeWindow());
		auto State = glfwGetKey(Window, static_cast<int>(vKey)); //return GLFW_PRESS/GLFW_RELEASE/GLFW_REPEAT
		return State == GLFW_PRESS || State == GLFW_REPEAT;
	}
	bool Input::isMouseButtonPressed(Mouse::MouseCode vButton) {
		auto* Window = reinterpret_cast<GLFWwindow*>(Application::GetInstance().getWindow().getNativeWindow());
		auto State = glfwGetMouseButton(Window, static_cast<int>(vButton)); //return GLFW_PRESS/GLFW_RELEASE
		return State == GLFW_PRESS;
	}
	glm::vec2 Input::getMousePosition() {
		auto* Window = reinterpret_cast<GLFWwindow*>(Application::GetInstance().getWindow().getNativeWindow());
		double Xpos, Ypos;
		glfwGetCursorPos(Window, &Xpos, &Ypos);
		return { static_cast<float>(Xpos), static_cast<float>(Ypos) };
	}
	float Input::getMouseX() {
		return getMousePosition().x;
	}
	float Input::getMouseY() {
		return getMousePosition().y;
	}


}