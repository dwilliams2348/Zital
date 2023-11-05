#pragma once

#ifdef ZT_PLATFORM_WINDOWS

extern Zital::Application* Zital::CreateApplication();

int main(int argc, char* argv)
{
	auto app = Zital::CreateApplication();
	app->Run();
	delete app;
}

#else
#error Zital only supports Windows as of now.
#endif