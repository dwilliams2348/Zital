#include "ZTpch.h"
#include "Application.h"

#include "Zital/Core/Log.h"

#include "Zital/Renderer/Renderer.h"

#include "Zital/Core/Input.h"

#include <GLFW/glfw3.h>

namespace Zital
{

#define BIND_EVENT_FN(x) std::bind(&x, this, std::placeholders::_1)

	Application* Application::sInstance = nullptr;

	Application::Application()
	{
		ZT_PROFILE_FUNCTION();

		ZT_CORE_ASSERT(!sInstance, "Application already exists.");
		sInstance = this;

		mWindow = Scope<Window>(Window::Create());
		mWindow->SetEventCallback(BIND_EVENT_FN(Application::OnEvent));

		Renderer::Init();

		mImGuiLayer = new ImGuiLayer();
		PushOverlay(mImGuiLayer);
	}

	Application::~Application()
	{
		ZT_PROFILE_FUNCTION();

		Renderer::Shutdown();
	}

	void Application::PushLayer(Layer* _layer)
	{
		ZT_PROFILE_FUNCTION();

		mLayerStack.PushLayer(_layer);
		_layer->OnAttach();
	}

	void Application::PushOverlay(Layer* _overlay)
	{
		ZT_PROFILE_FUNCTION();

		mLayerStack.PushOverlay(_overlay);
		_overlay->OnAttach();
	}

	void Application::OnEvent(Event& e)
	{
		EventDispatcher dispatcher(e);
		dispatcher.Dispatch<WindowCloseEvent>(BIND_EVENT_FN(Application::OnWindowClosed));
		dispatcher.Dispatch<WindowResizeEvent>(BIND_EVENT_FN(Application::OnWindowResized));
		
		for (auto it = mLayerStack.end(); it != mLayerStack.begin(); )
		{
			(*--it)->OnEvent(e);
			if (e.Handled)
				break;
		}
	}

	void Application::Run()
	{
		ZT_PROFILE_FUNCTION();

		while (mRunning)
		{
			ZT_PROFILE_SCOPE("Application Runloop");

			float time = (float)glfwGetTime(); //Platform::GetTime
			Timestep deltaTime = time - mLastFrameTime;
			mLastFrameTime = time;

			if (!mMinimized)
			{
				{
					ZT_PROFILE_SCOPE("LayerStack OnUpdate");

					for (Layer* layer : mLayerStack)
						layer->OnUpdate(deltaTime);
				}
			}

			mImGuiLayer->Begin();
			{
				ZT_PROFILE_SCOPE("LayerStack OnImGuiRender");

				for (Layer* layer : mLayerStack)
					layer->OnImGuiRender();
			}
			mImGuiLayer->End();

			mWindow->OnUpdate();
		}
	}

	bool Application::OnWindowClosed(WindowCloseEvent& e)
	{
		mRunning = false;

		return true;
	}

	bool Application::OnWindowResized(WindowResizeEvent& e)
	{
		ZT_PROFILE_FUNCTION();

		if (e.GetWidth() == 0 || e.GetHeight() == 0)
		{
			mMinimized = true;

			return false;
		}

		mMinimized = false;
		Renderer::OnWindowResize(e.GetWidth(), e.GetHeight());

		return false;
	}

}