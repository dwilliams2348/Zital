#pragma once

#include "Zital/Renderer/GraphicsContext.h"

struct GLFWwindow;

namespace Zital
{

	class OpenGLContext : public GraphicsContext
	{
	public:
		OpenGLContext(GLFWwindow* _windowHandle);

		virtual void Init() override;
		virtual void SwapBuffers() override;

	private:
		GLFWwindow* mWindowHandle;
	};

}