#include "pkpch.h"
#include "WindowsWindow.h"
#include "Pika/Events/ApplicationEvent.h"
#include "Pika/Events/KeyboardEvent.h"
#include "Pika/Events/MouseEvent.h"
#include "Pika/Core/KeyCodes.h"
#include "Pika/Core/MouseCodes.h"
#include <glad/glad.h>

namespace Pika {

	static Key::KeyCode GLFWToPikaKeyCode(int vKeyCode) {
		return static_cast<Key::KeyCode>(vKeyCode);	   // KeyCode就是GLFW版本
	}

	static Mouse::MouseCode GLFWToPikaMouseCode(int vButton) {
		return static_cast<Mouse::MouseCode>(vButton); // MouseCode就是GLFW版本
	}

	Window* Window::Create(const WindowProps& vWindowProps) {
		return new WindowsWindow(vWindowProps);
	}

	static uint8_t s_GLFWWindowCount = 0;

	static void GLFWErrorCallBack(int error_code, const char* description) {
		PK_CORE_ERROR("GLFW error ({0}): {1}", error_code, description);
	}

	WindowsWindow::WindowsWindow(const WindowProps& vWindowProps)
	{
		Initialize(vWindowProps);
	}
	WindowsWindow::~WindowsWindow()
	{
		PK_PROFILE_FUNCTION();

		shutDown();
	}
	void WindowsWindow::onUpdate()
	{
		PK_PROFILE_FUNCTION();

		glfwPollEvents();
		m_pContext->swapBuffer();
	}

	void WindowsWindow::setVSync(bool vEnable)
	{
		PK_PROFILE_FUNCTION();

		if (vEnable) {
			glfwSwapInterval(1);
		}
		else {
			glfwSwapInterval(0);
		}
		m_Data.m_VSync = vEnable;
	}

	bool WindowsWindow::isVSync() const
	{
		return m_Data.m_VSync;
	}

	void WindowsWindow::Initialize(const WindowProps& vWindowProps)
	{
		PK_PROFILE_FUNCTION();

		{
			PK_PROFILE_SCOPE("WindowsWindow : Window set up");

			m_Data.m_Title = vWindowProps.m_Title;
			m_Data.m_Width = vWindowProps.m_Width;
			m_Data.m_Height = vWindowProps.m_Height;
			PK_CORE_INFO("Creating Window {0} ({1}, {2})", m_Data.m_Title, m_Data.m_Width, m_Data.m_Height);
			if (s_GLFWWindowCount == 0) {
				int Success = glfwInit();
				PK_CORE_ASSERT(Success, "Could not initialize GLFW!");
				glfwSetErrorCallback(GLFWErrorCallBack);
			}

			m_pWindow = glfwCreateWindow(static_cast<int>(vWindowProps.m_Width), static_cast<int>(vWindowProps.m_Height),
				vWindowProps.m_Title.c_str(), nullptr, nullptr);

			m_pContext = GraphicsContext::Create(m_pWindow);
			PK_ASSERT(m_pContext, "WindowsWindow : m_pContext is nullptr!");
			m_pContext->Initialize();

			glfwSetWindowUserPointer(m_pWindow, &m_Data);
			setVSync(true);
		}

		{
			PK_PROFILE_SCOPE("WindowsWindow : Window callbacks set up");

			glfwSetWindowSizeCallback(m_pWindow, [](GLFWwindow* window, int width, int height)
				{
					WindowData& Data = *reinterpret_cast<WindowData*>(glfwGetWindowUserPointer(window));
					Data.m_Width = width;
					Data.m_Height = height;
					WindowResizeEvent Event(width, height);
					Data.eventCallBack(Event);
				});

			glfwSetWindowCloseCallback(m_pWindow, [](GLFWwindow* window)
				{
					WindowData& Data = *reinterpret_cast<WindowData*>(glfwGetWindowUserPointer(window));
					WindowCloseEvent Event;
					Data.eventCallBack(Event);
				});

			glfwSetKeyCallback(m_pWindow, [](GLFWwindow* window, int key, int scancode, int action, int mods)
				{
					WindowData& Data = *reinterpret_cast<WindowData*>(glfwGetWindowUserPointer(window));
					switch (action)
					{
					case GLFW_PRESS:
					{
						KeyPressedEvent Event(GLFWToPikaKeyCode(key));
						Data.eventCallBack(Event);
						break;
					}
					case GLFW_RELEASE:
					{
						KeyReleasedEvent Event(GLFWToPikaKeyCode(key));
						Data.eventCallBack(Event);
						break;
					}
					case GLFW_REPEAT:
					{
						KeyPressedEvent Event(GLFWToPikaKeyCode(key), 1);
						Data.eventCallBack(Event);
						break;
					}
					}
				});

			glfwSetCharCallback(m_pWindow, [](GLFWwindow* window, unsigned int codepoint)
				{
					WindowData& Data = *reinterpret_cast<WindowData*>(glfwGetWindowUserPointer(window));
					KeyTypedEvent Event(GLFWToPikaKeyCode(codepoint)); //codepoint is keycode
					Data.eventCallBack(Event);
				});

			glfwSetMouseButtonCallback(m_pWindow, [](GLFWwindow* window, int button, int action, int mods)
				{
					WindowData& Data = *reinterpret_cast<WindowData*>(glfwGetWindowUserPointer(window));
					switch (action)
					{
					case GLFW_PRESS:
					{
						MouseButtonPressedEvent Event(GLFWToPikaMouseCode(button));
						Data.eventCallBack(Event);
						break;
					}
					case GLFW_RELEASE:
					{
						MouseButtonRleasedEvent Event(GLFWToPikaMouseCode(button));
						Data.eventCallBack(Event);
						break;
					}
					}
				});

			glfwSetScrollCallback(m_pWindow, [](GLFWwindow* window, double xoffset, double yoffset)
				{
					WindowData& Data = *reinterpret_cast<WindowData*>(glfwGetWindowUserPointer(window));
					MouseScrolledEvent Event(static_cast<float>(xoffset), static_cast<float>(yoffset));
					Data.eventCallBack(Event);
				});

			glfwSetCursorPosCallback(m_pWindow, [](GLFWwindow* window, double xpos, double ypos)
				{
					WindowData& Data = *reinterpret_cast<WindowData*>(glfwGetWindowUserPointer(window));
					MouseMovedEvent Event(static_cast<float>(xpos), static_cast<float>(ypos));
					Data.eventCallBack(Event);
				});
		}
	}

	void WindowsWindow::shutDown()
	{
		PK_PROFILE_FUNCTION();

		glfwDestroyWindow(m_pWindow);
		--s_GLFWWindowCount;
		if (s_GLFWWindowCount == 0)
		{
			glfwTerminate();
		}
	}

}