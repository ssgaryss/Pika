#include "pkpch.h"
#include "WindowsWindow.h"
#include "Pika/Events/ApplicationEvent.h"
#include "Pika/Events/KeyboardEvent.h"
#include "Pika/Events/MouseEvent.h"

#include <glad/glad.h>

namespace Pika {

	Window* Window::create(const WindowProps& vWindowProps) {
		return new WindowsWindow(vWindowProps);
	}

	static uint8_t s_GLFWWindowCount = 0;

	static void GLFWErrorCallBack(int error_code, const char* description) {
		PK_CORE_ERROR("GLFW error ({0}): {1}", error_code, description);
	}

	WindowsWindow::WindowsWindow(const WindowProps& vWindowProps)
	{
		init(vWindowProps);
	}
	WindowsWindow::~WindowsWindow()
	{
		shutDown();
	}
	void WindowsWindow::onUpdate()
	{
		glfwPollEvents();
		glfwSwapBuffers(m_Window);
	}
	unsigned int Pika::WindowsWindow::getWidth() const
	{
		return m_Data.m_Width;
	}

	unsigned int WindowsWindow::getHeight() const
	{
		return m_Data.m_Height;
	}

	void* WindowsWindow::getNativeWindow() const
	{
		return m_Window;
	}

	void WindowsWindow::setVSync(bool vEnable)
	{
		if (vEnable) {
			glfwSwapInterval(1);
		}
		else {
			glfwSwapInterval(0);
		}
		m_Data.VSync = vEnable;
	}

	bool WindowsWindow::isVSync() const
	{
		return m_Data.VSync;
	}

	void WindowsWindow::init(const WindowProps& vWindowProps)
	{
		m_Data.m_Title = vWindowProps.m_Title;
		m_Data.m_Width = vWindowProps.m_Width;
		m_Data.m_Height = vWindowProps.m_Height;
		PK_CORE_INFO("Creating Window {0} ({1}, {2})", m_Data.m_Title, m_Data.m_Width, m_Data.m_Height);
		if (s_GLFWWindowCount == 0) {
			int Success = glfwInit();
			PK_CORE_ASSERT(Success, "Could not initialize GLFW!");
			glfwSetErrorCallback(GLFWErrorCallBack);
		}

		m_Window = glfwCreateWindow(static_cast<int>(vWindowProps.m_Width), static_cast<int>(vWindowProps.m_Height),
			vWindowProps.m_Title.c_str(), nullptr, nullptr);
		glfwMakeContextCurrent(m_Window);
		int Status = gladLoadGLLoader((GLADloadproc)glfwGetProcAddress);
		PK_CORE_ASSERT(Status, "Could not initialize gald!");
		glfwSetWindowUserPointer(m_Window, &m_Data);
		setVSync(true);

		glfwSetWindowSizeCallback(m_Window, [](GLFWwindow* window, int width, int height)
			{
				WindowData& Data = *reinterpret_cast<WindowData*>(glfwGetWindowUserPointer(window));
				Data.m_Width = width;
				Data.m_Height = height;
				WindowResizeEvent Event(width, height);
				Data.eventCallBack(Event);
			});

		glfwSetWindowCloseCallback(m_Window, [](GLFWwindow* window)
			{
				WindowData& Data = *reinterpret_cast<WindowData*>(glfwGetWindowUserPointer(window));
				WindowCloseEvent Event;
				Data.eventCallBack(Event);
			});

		glfwSetKeyCallback(m_Window, [](GLFWwindow* window, int key, int scancode, int action, int mods)
			{
				WindowData& Data = *reinterpret_cast<WindowData*>(glfwGetWindowUserPointer(window));
				switch (action)
				{
				case GLFW_PRESS:
				{
					KeyPressedEvent Event(key, 0);
					Data.eventCallBack(Event);
					break;
				}
				case GLFW_RELEASE:
				{
					KeyReleasedEvent Event(key);
					Data.eventCallBack(Event);
					break;
				}
				case GLFW_REPEAT:
				{
					KeyPressedEvent Event(key, 1);
					Data.eventCallBack(Event);
					break;
				}
				}
			});

		glfwSetCharCallback(m_Window, [](GLFWwindow* window, unsigned int codepoint)
			{
				WindowData& Data = *reinterpret_cast<WindowData*>(glfwGetWindowUserPointer(window));
				KeyTypedEvent Event(codepoint); //codepoint is keycode
				Data.eventCallBack(Event);
			});

		glfwSetMouseButtonCallback(m_Window, [](GLFWwindow* window, int button, int action, int mods)
			{
				WindowData& Data = *reinterpret_cast<WindowData*>(glfwGetWindowUserPointer(window));
				switch (action)
				{
				case GLFW_PRESS:
				{
					MouseButtonPressedEvent Event(button);
					Data.eventCallBack(Event);
					break;
				}
				case GLFW_RELEASE:
				{
					MouseButtonRleasedEvent Event(button);
					Data.eventCallBack(Event);
					break;
				}
				}
			});

		glfwSetScrollCallback(m_Window, [](GLFWwindow* window, double xoffset, double yoffset)
			{
				WindowData& Data = *reinterpret_cast<WindowData*>(glfwGetWindowUserPointer(window));
				MouseScrolledEvent Event(static_cast<float>(xoffset), static_cast<float>(yoffset));
				Data.eventCallBack(Event);
			});

		glfwSetCursorPosCallback(m_Window, [](GLFWwindow* window, double xpos, double ypos)
			{
				WindowData& Data = *reinterpret_cast<WindowData*>(glfwGetWindowUserPointer(window));
				MouseMovedEvent Event(static_cast<float>(xpos), static_cast<float>(ypos));
				Data.eventCallBack(Event);
			});
	}

	void WindowsWindow::shutDown()
	{
		glfwDestroyWindow(m_Window);
		--s_GLFWWindowCount;
		if (s_GLFWWindowCount == 0)
		{
			glfwTerminate();
		}
	}

}