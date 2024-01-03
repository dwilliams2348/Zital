#pragma once

#include "Core.h"

#include "Window.h"
#include "Zital/Core/LayerStack.h"
#include "Zital/Events/Event.h"
#include "Zital/Events/ApplicationEvent.h"

#include "Zital/Core/Timestep.h"

#include "Zital/ImGui/ImGuiLayer.h"

namespace Zital 
{

	struct ApplicationCommandLineArgs
	{
		int Count = 0;
		char** Args = nullptr;

		const char* operator[](int _index) const
		{
			ZT_CORE_ASSERT(_index < count, "Invalid index");
			return Args[_index];
		}
	};

	class Application
	{
	public:
		Application(const std::string& _name = "Zital App", ApplicationCommandLineArgs _args = ApplicationCommandLineArgs());
		virtual ~Application();

		void Run();

		void OnEvent(Event& e);

		void PushLayer(Layer* _layer);
		void PushOverlay(Layer* _overlay);

		ImGuiLayer* GetImGuiLayer() { return mImGuiLayer; }

		inline static Application& Get() { return *sInstance; }
		inline Window& GetWindow() { return *mWindow; }

		ApplicationCommandLineArgs GetCommandLineArgs() const { return mCommandLineArgs; }

		void Close();

	private:
		bool OnWindowClosed(WindowCloseEvent& e);
		bool OnWindowResized(WindowResizeEvent& e);

	private:
		ApplicationCommandLineArgs mCommandLineArgs;
		Scope<Window> mWindow;
		ImGuiLayer* mImGuiLayer;
		bool mRunning = true;
		bool mMinimized = false;
		LayerStack mLayerStack;
		float mLastFrameTime = 0.f;


	private:
		static Application* sInstance;
	};

	//to be defined in client
	Application* CreateApplication(ApplicationCommandLineArgs _args);

}