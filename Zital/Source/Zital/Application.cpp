#include "ZTpch.h"
#include "Application.h"

#include "Zital/Events/ApplicationEvent.h"
#include "Zital/Log.h"

namespace Zital
{

	Application::Application()
	{
		mWindow = std::unique_ptr<Window>(Window::Create());
	}

	Application::~Application()
	{

	}

	void Application::Run()
	{
		while (mRunning)
		{
			mWindow->OnUpdate();
		}
	}

}