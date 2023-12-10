#pragma once

#ifdef ZT_PLATFORM_WINDOWS

extern Zital::Application* Zital::CreateApplication();

int main(int argc, char* argv)
{
	Zital::Log::Init();
	ZT_CORE_WARN("Initialized logger");
	//int a = 5;
	//ZT_INFO("Hello Var={0}", a);

	auto app = Zital::CreateApplication();
	app->Run();
	delete app;
}

#else
#error Zital only supports Windows as of now.
#endif