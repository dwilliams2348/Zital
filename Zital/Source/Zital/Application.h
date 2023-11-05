#pragma once

#include "Core.h"

namespace Zital 
{

	class ZITAL_API Application
	{
	public:
		Application();
		virtual ~Application();

		void Run();
	};

	//to be defined in client
	Application* CreateApplication();

}