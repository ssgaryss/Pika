#include "pkpch.h"
#include "OpenGLContext.h"
#include <glad/glad.h>
#include <GLFW/glfw3.h>

namespace Pika {

	OpenGLContext::OpenGLContext(GLFWwindow* vWindow) :
		m_pWindowHandle{ vWindow }
	{
	}

	void OpenGLContext::Initialize()
	{
		PK_PROFILE_FUNCTION();

		glfwMakeContextCurrent(m_pWindowHandle);
		int Status = gladLoadGLLoader((GLADloadproc)glfwGetProcAddress);
		PK_CORE_ASSERT(Status, "Could not initialize gald!");

		m_Information.m_Vendor = reinterpret_cast<const char*>(glad_glGetString(GL_VENDOR));
		m_Information.m_Renderer = reinterpret_cast<const char*>(glad_glGetString(GL_RENDERER));
		m_Information.m_Version = reinterpret_cast<const char*>(glad_glGetString(GL_VERSION));

		PK_CORE_INFO("OpenGL Info :");
		PK_CORE_INFO("     Vendor : {0}", m_Information.m_Renderer);
		PK_CORE_INFO("   Renderer : {0}", m_Information.m_Vendor);
		PK_CORE_INFO("    Version : {0}", m_Information.m_Version);
	}

	void OpenGLContext::swapBuffer()
	{
		PK_PROFILE_FUNCTION();

		glfwSwapBuffers(m_pWindowHandle);
	}

}