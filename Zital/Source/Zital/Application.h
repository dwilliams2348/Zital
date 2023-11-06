#pragma once

#include "Core.h"
#include "Events/Event.h"
#include "Zital/Events/ApplicationEvent.h"

#include "Window.h"

namespace Zital 
{

	class ZITAL_API Application
	{
	public:
		Application();
		virtual ~Application();

		void Run();

		void OnEvent(Event& e);

	private:
		bool OnWindowClosed(WindowCloseEvent& e);

		std::unique_ptr<Window> mWindow;
		bool mRunning = true;
	};

	//to be defined in client
	Application* CreateApplication();

}