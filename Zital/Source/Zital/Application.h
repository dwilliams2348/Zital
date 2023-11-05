#pragma once

#include "Core.h"
#include "Events/Event.h"
#include "Window.h"

namespace Zital 
{

	class ZITAL_API Application
	{
	public:
		Application();
		virtual ~Application();

		void Run();

	private:
		std::unique_ptr<Window> mWindow;
		bool mRunning = true;
	};

	//to be defined in client
	Application* CreateApplication();

}