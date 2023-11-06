#pragma once

#include "ZTpch.h"
#include "Zital/Core.h"

namespace Zital
{
	/*
	Events in Zital are currently blocking events, which means when the event occurs it
	is immeditatele dispatched and handled right then and there.
	In the future this will dealt with in a buffer where the events get stored on a buffer
	and are handled during an event part of the update stage
	*/

	/*
	There are different event types that take place in the Zital event system.
	They will all have their type labeled to know how to handle the event.
	*/
	enum class EEventType
	{
		None = 0,
		WindowClose, WindowResize, WindowFocus, WindowLostFocus, WindowMoved,
		AppTick, AppUpdate, AppRender,
		KeyPressed, KeyReleased,
		MouseButtonPressed, MouseButtonReleased, MouseMoved, MouseScrolled
	};

	/*
	Event Categories are stored in a bit field since events can belong to multiple categories.
	*/
	enum EEventCategory
	{
		None = 0,
		EventCategoryApplication    = BIT(0),
		EventCategoryInput          = BIT(1),
		EventCategoryKeyboard       = BIT(2),
		EventCategoryMouse          = BIT(3),
		EventCategoryMouseButton    = BIT(4)
	};

#define EVENT_CLASS_TYPE(type) static EEventType GetStaticType(){return EEventType::##type;}\
                               virtual EEventType GetEventType() const override {return GetStaticType();}\
                               virtual const char* GetName() const override {return #type;}

#define EVENT_CLASS_CATEGORY(category) virtual int GetCategoryFlags() const override {return category;}

	class ZITAL_API Event
	{
		friend class EventDispatcher;

	public:
		bool Handled = false;

		virtual EEventType GetEventType() const = 0;
		virtual const char* GetName() const = 0;
		virtual int GetCategoryFlags() const = 0;
		virtual std::string ToString() const { return GetName(); }

		inline bool IsInCategory(EEventCategory _category)
		{
			return GetCategoryFlags() & _category;
		}
	};

	class EventDispatcher
	{
		template<typename T>
		using EventFn = std::function<bool(T&)>;

	public:
		EventDispatcher(Event& _event)
			: mEvent(_event)
		{
		}

		//Dispatches a fucntion for a given event, function must return a bool
		template<typename T>
		bool Dispatch(EventFn<T> func)
		{
			if (mEvent.GetEventType() == T::GetStaticType())
			{
				mEvent.Handled = func(*(T*)&mEvent);
				return true;
			}

			return false;
		}

	private:
		Event& mEvent;
	};

	inline std::ostream& operator<<(std::ostream& os, const Event& e)
	{
		return os << e.ToString();
	}
}