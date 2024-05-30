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

		inline std::string getTitle() const override { return m_Data.m_Title; }
		inline unsigned int getWidth() const override { return m_Data.m_Width; }
		inline unsigned int getHeight() const override { return m_Data.m_Height; }
		inline void* getNativeWindow() const override { return m_pWindow; }

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
			bool m_VSync; //vetical synchronization

			EventCallbackFn eventCallBack;
		};
		WindowData m_Data;
		GLFWwindow* m_pWindow;
		std::shared_ptr<GraphicsContext> m_pContext;

	};
}

