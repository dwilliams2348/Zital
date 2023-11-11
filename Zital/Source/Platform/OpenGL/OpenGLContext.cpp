#include "ZTpch.h"
#include "OpenGLContext.h"

#include <GLFW/glfw3.h>
#include <glad/glad.h>

namespace Zital
{

	OpenGLContext::OpenGLContext(GLFWwindow* _windowHandle)
		: mWindowHandle(_windowHandle)
	{
		ZT_CORE_ASSERT(_windowHandle, "Window handle is null.")
	}

	void OpenGLContext::Init()
	{
		glfwMakeContextCurrent(mWindowHandle);
		int status = gladLoadGLLoader((GLADloadproc)glfwGetProcAddress);
		ZT_CORE_ASSERT(status, "Failed to initialize Glad.");

		ZT_CORE_INFO("OpenGL Info:");
		ZT_CORE_INFO("  Vendor: {0}", (char*)glGetString(GL_VENDOR));
		ZT_CORE_INFO("  Renderer: {0}", (char*)glGetString(GL_RENDERER));
		ZT_CORE_INFO("  Version: {0}", (char*)glGetString(GL_VERSION));
	}

	void OpenGLContext::SwapBuffers()
	{
		glfwSwapBuffers(mWindowHandle);
	}

}