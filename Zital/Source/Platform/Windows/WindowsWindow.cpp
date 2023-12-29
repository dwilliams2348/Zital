#include "ZTpch.h"
#include "WindowsWindow.h"

#include "Zital/Events/ApplicationEvent.h"
#include "Zital/Events/KeyEvent.h"
#include "Zital/Events/MouseEvent.h"

#include "Platform/OpenGL/OpenGLContext.h"

namespace Zital
{
	static bool sGLFWInitialized = false;

	static void GLFWErrorCallback(int _error, const char* _description)
	{
		ZT_CORE_ERROR("GLFW Error ({0}): {1}", _error, _description);
	}

	Window* Window::Create(const SWindowProps& _props)
	{
		return new WindowsWindow(_props);
	}

	WindowsWindow::WindowsWindow(const SWindowProps& _props)
	{
		ZT_PROFILE_FUNCTION();

		Init(_props);
	}

	WindowsWindow::~WindowsWindow()
	{
		Shutdown();
	}

	void WindowsWindow::Init(const SWindowProps& _props)
	{
		ZT_PROFILE_FUNCTION();

		mData.Title = _props.Title;
		mData.Width = _props.Width;
		mData.Height = _props.Height;

		ZT_CORE_INFO("Creating window {0} ({1}, {2})", _props.Title, _props.Width, _props.Height);

		if (!sGLFWInitialized)
		{
			//TODO: glfwTerminate on system shutdown
			int success = glfwInit();
			ZT_CORE_ASSERT(success, "Could not initialize GLFW.");
			glfwSetErrorCallback(GLFWErrorCallback);

			sGLFWInitialized = true;
		}

		mWindow = glfwCreateWindow((int)_props.Width, (int)_props.Height, mData.Title.c_str(), nullptr, nullptr);

		mContext = CreateScope<OpenGLContext>(mWindow);
		mContext->Init();

		glfwSetWindowUserPointer(mWindow, &mData);
		SetVSync(true);

		//set GLFW callbacks
		glfwSetWindowSizeCallback(mWindow, [](GLFWwindow* _window, int _width, int _height)
			{
				SWindowData& data = *(SWindowData*)glfwGetWindowUserPointer(_window);
				data.Width = _width;
				data.Height = _height;

				WindowResizeEvent event(_width, _height);
				data.EventCallback(event);

			});

		glfwSetWindowCloseCallback(mWindow, [](GLFWwindow* _window)
			{
				SWindowData& data = *(SWindowData*)glfwGetWindowUserPointer(_window);
				WindowCloseEvent event;
				data.EventCallback(event);
			});

		glfwSetKeyCallback(mWindow, [](GLFWwindow* _window, int _key, int _scancode, int _action, int _mods)
			{
				SWindowData& data = *(SWindowData*)glfwGetWindowUserPointer(_window);

				switch (_action)
				{
					case GLFW_PRESS:
					{
						KeyPressedEvent event(static_cast<KeyCode>(_key), 0);
						data.EventCallback(event);
						break;
					}
					case GLFW_RELEASE:
					{
						KeyReleasedEvent event(static_cast<KeyCode>(_key));
						data.EventCallback(event);
						break;
					}
					case GLFW_REPEAT:
					{
						KeyPressedEvent event(static_cast<KeyCode>(_key), 1);
						data.EventCallback(event);
						break;
					}
				}
			});

		glfwSetCharCallback(mWindow, [](GLFWwindow* _window, unsigned _keycode)
			{
				SWindowData& data = *(SWindowData*)glfwGetWindowUserPointer(_window);

				KeyTypedEvent event(static_cast<KeyCode>(_keycode));
				data.EventCallback(event);
			});

		glfwSetMouseButtonCallback(mWindow, [](GLFWwindow* _window, int _button, int _action, int _mods)
			{
				SWindowData& data = *(SWindowData*)glfwGetWindowUserPointer(_window);

				switch (_action)
				{
					case GLFW_PRESS:
					{
						MouseButtonPressedEvent event(static_cast<MouseCode>(_button));
						data.EventCallback(event);
						break;
					}
					case GLFW_RELEASE:
					{
						MouseButtonReleasedEvent event(static_cast<MouseCode>(_button));
						data.EventCallback(event);
						break;
					}
				}
			});

		glfwSetScrollCallback(mWindow, [](GLFWwindow* _window, double _xOffset, double _yOffset)
			{
				SWindowData& data = *(SWindowData*)glfwGetWindowUserPointer(_window);
				
				MouseScrolledEvent event((float)_xOffset, (float)_yOffset);
				data.EventCallback(event);
			});

		glfwSetCursorPosCallback(mWindow, [](GLFWwindow* _window, double _xPos, double _yPos)
			{
				SWindowData& data = *(SWindowData*)glfwGetWindowUserPointer(_window);

				MouseMovedEvent event((float)_xPos, (float)_yPos);
				data.EventCallback(event);
			});
	}

	void WindowsWindow::Shutdown()
	{
		ZT_PROFILE_FUNCTION();

		glfwDestroyWindow(mWindow);
	}

	void WindowsWindow::OnUpdate()
	{
		ZT_PROFILE_FUNCTION();

		glfwPollEvents();
		mContext->SwapBuffers();
	}

	void WindowsWindow::SetVSync(bool _enabled)
	{
		ZT_PROFILE_FUNCTION();

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