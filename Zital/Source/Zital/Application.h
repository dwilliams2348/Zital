#pragma once

#include "Core.h"

#include "Window.h"
#include "Zital/LayerStack.h"
#include "Events/Event.h"
#include "Events/ApplicationEvent.h"

#include "ImGui/ImGuiLayer.h"

#include "Renderer/Shader.h"
#include "Renderer/Buffer.h"

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

		std::unique_ptr<Window> mWindow;
		ImGuiLayer* mImGuiLayer;
		bool mRunning = true;
		LayerStack mLayerStack;

		unsigned mVertexArray;
		std::unique_ptr<Shader> mShader;
		std::unique_ptr<VertexBuffer> mVertexBuffer;
		std::unique_ptr<IndexBuffer> mIndexBuffer;

		static Application* sInstance;
	};

	//to be defined in client
	Application* CreateApplication();

}