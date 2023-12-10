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

	class ZITAL_API Application
	{
	public:
		Application();
		virtual ~Application();

		void Run();

		void OnEvent(Event& e);

		void PushLayer(Layer* _layer);
		void PushOverlay(Layer* _overlay);

		inline static Application& Get() { return *sInstance; }
		inline Window& GetWindow() { return* mWindow; }

	private:
		bool OnWindowClosed(WindowCloseEvent& e);
		bool OnWindowResized(WindowResizeEvent& e);

	private:
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
	Application* CreateApplication();

}