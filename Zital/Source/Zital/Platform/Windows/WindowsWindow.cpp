#include "ZTpch.h"
#include "WindowsWindow.h"

namespace Zital
{
	static bool sGLFWInitialized = false;

	Window* Window::Create(const SWindowProps& _props)
	{
		return new WindowsWindow(_props);
	}

	WindowsWindow::WindowsWindow(const SWindowProps& _props)
	{
		Init(_props);
	}

	WindowsWindow::~WindowsWindow()
	{
		Shutdown();
	}

	void WindowsWindow::Init(const SWindowProps& _props)
	{
		mData.Title = _props.Title;
		mData.Width = _props.Width;
		mData.Height = _props.Height;

		ZT_CORE_INFO("Creating window {0} ({1}, {2})", _props.Title, _props.Width, _props.Height);

		if (!sGLFWInitialized)
		{
			//TODO: glfwTerminate on system shutdown
			int success = glfwInit();
			ZT_CORE_ASSERT(success, "Could not initialize GLFW.");

			sGLFWInitialized = true;
		}

		mWindow = glfwCreateWindow((int)_props.Width, (int)_props.Height, mData.Title.c_str(), nullptr, nullptr);
		glfwMakeContextCurrent(mWindow);
		glfwSetWindowUserPointer(mWindow, &mData);
		SetVSync(true);
	}

	void WindowsWindow::Shutdown()
	{
		glfwDestroyWindow(mWindow);
	}

	void WindowsWindow::OnUpdate()
	{
		glfwPollEvents();
		glfwSwapBuffers(mWindow);
	}

	void WindowsWindow::SetVSync(bool _enabled)
	{
		if (_enabled)
			glfwSwapInterval(1);
		else
			glfwSwapInterval(0);

		mData.VSync = _enabled;
	}

	bool WindowsWindow::IsVSync() const
	{
		return mData.VSync;
	}

}