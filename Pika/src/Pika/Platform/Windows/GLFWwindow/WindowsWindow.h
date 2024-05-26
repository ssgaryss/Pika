#pragma once

#include "Pika/Core/Window.h"
#include "Pika/Renderer/GraphicsContext.h"
#include <GLFW/glfw3.h>

#include "Pika/Platform/OpenGL/OpenGLContext.h"

namespace Pika {

	class WindowsWindow : public Window
	{
	public:
		WindowsWindow(const WindowProps& vWindowProps);
		~WindowsWindow();
		void onUpdate() override;
		unsigned int getWidth() const override;
		unsigned int getHeight() const override;
		void* getNativeWindow() const override;

		inline void setEventCallback(const EventCallbackFn& vEventCallback) override { m_Data.eventCallBack = vEventCallback; };
		void setVSync(bool vEnable) override;
		bool isVSync() const override;
	private:
		void init(const WindowProps& vWindowProps);
		void shutDown();
	private:
		struct WindowData {
			std::string m_Title;
			unsigned int m_Width, m_Height;
			bool VSync; //vetical synchronization

			EventCallbackFn eventCallBack;
		};
		GLFWwindow* m_Window;
		WindowData m_Data;
		OpenGLContext m_Context;

	};
}

