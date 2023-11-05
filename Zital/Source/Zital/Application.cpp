#include "ZTpch.h"
#include "Application.h"

#include "Zital/Events/ApplicationEvent.h"
#include "Zital/Log.h"

namespace Zital
{

	Application::Application()
	{

	}

	Application::~Application()
	{

	}

	void Application::Run()
	{
		WindowResizeEvent e(1280, 720);
		ZT_TRACE(e);

		while (true);
	}

}