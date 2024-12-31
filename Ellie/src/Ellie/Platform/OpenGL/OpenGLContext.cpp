#include "eepch.h"
#include "OpenGLContext.h"

#include "glad/glad.h"

namespace Ellie {

	OpenGLContext::OpenGLContext(GLFWwindow* windowHandle)
		: m_WindowHandle(windowHandle)
	{
		EE_ASSERT(m_WindowHandle, "Window handle is null!");
	}

	void OpenGLContext::Init()
	{
		glfwMakeContextCurrent(m_WindowHandle);
		int status = gladLoadGLLoader((GLADloadproc)glfwGetProcAddress);
		EE_ASSERT(status, "Failed to load Glad!");

		EE_CORE_INFO("Renderer: {0}", reinterpret_cast<const char*>(glGetString(GL_RENDERER)));
		EE_CORE_INFO("Vendor: {0}", reinterpret_cast<const char*>(glGetString(GL_VENDOR)));
		EE_CORE_INFO("OpenGL Version: {0}", reinterpret_cast<const char*>(glGetString(GL_VERSION)));
	}

	void OpenGLContext::SwapBuffers()
	{
		glfwSwapBuffers(m_WindowHandle);
	}

}