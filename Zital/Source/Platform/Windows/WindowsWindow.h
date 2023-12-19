#pragma once

#include "Zital/Core/Window.h"
#include "Zital/Renderer/GraphicsContext.h"

#include <GLFW/glfw3.h>


namespace Zital
{

	class WindowsWindow : public Window
	{
	public:
		WindowsWindow(const SWindowProps& _props);
		virtual ~WindowsWindow();

		void OnUpdate() override;

		inline unsigned GetWidth() const override { return mData.Width; }
		inline unsigned GetHeight() const override { return mData.Height; }

		inline void SetEventCallback(const EventCallbackFn& _callback) override { mData.EventCallback = _callback; }
		void SetVSync(bool _enable) override;
		bool IsVSync() const override;

		inline virtual void* GetNativeWindow() const { return mWindow; }

	private:
		virtual void Init(const SWindowProps& _props);
		virtual void Shutdown();

	private:
		GLFWwindow* mWindow;
		Scope<GraphicsContext> mContext;

		struct SWindowData
		{
			std::string Title;
			unsigned Width, Height;
			bool VSync;

			EventCallbackFn EventCallback;
		};

		SWindowData mData;
	};

}