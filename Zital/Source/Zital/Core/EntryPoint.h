#pragma once

#include "Zital/Core/Core.h"
#include "Zital/Core/Application.h"

#ifdef ZT_PLATFORM_WINDOWS

extern Zital::Application* Zital::CreateApplication(ApplicationCommandLineArgs _args);

int main(int argc, char** argv)
{
	Zital::Log::Init();
	
	ZT_PROFILE_BEGIN_SESSION("Startup", "ZitalProfile-Startup.json");
	auto app = Zital::CreateApplication({ argc, argv });
	ZT_PROFILE_END_SESSION();

	ZT_PROFILE_BEGIN_SESSION("Runtime", "ZitalProfile-Runtime.json");
	app->Run();
	ZT_PROFILE_END_SESSION();

	ZT_PROFILE_BEGIN_SESSION("Shutdown", "ZitalProfile-Shutdown.json");
	delete app;
	ZT_PROFILE_END_SESSION();
}

#else
#	error Zital only supports Windows as of now.
#endif