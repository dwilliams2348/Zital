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
	}

	void OpenGLContext::SwapBuffers()
	{
		glfwSwapBuffers(mWindowHandle);
	}

}