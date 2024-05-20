#include "pkpch.h"
#include "WindowsWindow.h"

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
		if (s_GLFWWindowCount == 0) {
			int Success = glfwInit();
			PK_CORE_ASSERT(Success, "Could not initialze GLFW!");
			glfwSetErrorCallback(GLFWErrorCallBack);
		}

		m_Window = glfwCreateWindow(static_cast<int>(vWindowProps.m_Width), static_cast<int>(vWindowProps.m_Height), 
									vWindowProps.m_Title.c_str(), nullptr, nullptr);
		glfwMakeContextCurrent(m_Window);
		glfwSetWindowUserPointer(m_Window, &m_Data);
		setVSync(true);

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