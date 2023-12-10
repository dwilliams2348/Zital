#pragma once

#include "ZTpch.h"

#include "Zital/Core/Core.h"
#include "Zital/Events/Event.h"

namespace Zital
{

	struct SWindowProps
	{
		std::string Title;
		unsigned Width;
		unsigned Height;

		SWindowProps(const std::string& _title = "Zital Engine",
			unsigned _width = 1280,
			unsigned _height = 720)
			: Title(_title), Width(_width), Height(_height) 
		{
		}
	};

	//Interface representing a desktop system based Window
	class ZITAL_API Window
	{
	public:
		using EventCallbackFn = std::function<void(Event&)>;

		virtual ~Window() {}

		virtual void OnUpdate() = 0;

		virtual unsigned GetWidth() const = 0;
		virtual unsigned GetHeight() const = 0;

		//Window attributes
		virtual void SetEventCallback(const EventCallbackFn& _callback) = 0;
		virtual void SetVSync(bool _enabled) = 0;
		virtual bool IsVSync() const = 0;

		virtual void* GetNativeWindow() const = 0;

		static Window* Create(const SWindowProps& _props = SWindowProps());
	};

}