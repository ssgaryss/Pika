#include "pkpch.h"
#include "OpenGLContext.h"
#include <glad/glad.h>
#include <GLFW/glfw3.h>

namespace Pika {

    OpenGLContext::OpenGLContext(GLFWwindow* vWindow):
        m_pWindowHandle{vWindow}
    {
    }

    void OpenGLContext::init()
    {
        glfwMakeContextCurrent(m_pWindowHandle);
        int Status = gladLoadGLLoader((GLADloadproc)glfwGetProcAddress);
        PK_CORE_ASSERT(Status, "Could not initialize gald!");

        PK_CORE_INFO("OpenGL Info :");
        PK_CORE_INFO("     Vendor : {0}", reinterpret_cast<const char*>(glad_glGetString(GL_VENDOR)));
        PK_CORE_INFO("   Renderer : {0}", reinterpret_cast<const char*>(glad_glGetString(GL_RENDERER)));
        PK_CORE_INFO("    Version : {0}", reinterpret_cast<const char*>(glad_glGetString(GL_VERSION)));
    }

    void OpenGLContext::swapBuffer()
    {
        glfwSwapBuffers(m_pWindowHandle);
    }

}