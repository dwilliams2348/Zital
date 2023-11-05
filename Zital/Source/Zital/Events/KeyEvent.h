#pragma once


#include "Event.h"

#include <sstream>

namespace Zital
{

	class ZITAL_API KeyEvent : public Event
	{
	public:
		inline int GetKeyCode() const { return mKeyCode; }

		EVENT_CLASS_CATEGORY(EventCategoryKeyboard | EventCategoryInput)

	protected:
		KeyEvent(int _keycode)
			: mKeyCode(_keycode) {}

		int mKeyCode;
	};

	class ZITAL_API KeyPressedEvent : public KeyEvent
	{
	public:
		KeyPressedEvent(int _keycode, int _repeatCount)
			: KeyEvent(_keycode), mRepeatCount(_repeatCount) {}

		inline int GetRepeatCount() { return mRepeatCount; }

		std::string ToString() const override
		{
			std::stringstream ss;
			ss << "KeyPressedEvent: " << mKeyCode << " (" << mRepeatCount << " repeats)";
			return ss.str();
		}

		EVENT_CLASS_TYPE(KeyPressed)

	private:
		int mRepeatCount;
	};

	class ZITAL_API KeyReleasedEvent : public KeyEvent
	{
	public:
		KeyReleasedEvent(int _keycode)
			: KeyEvent(_keycode) {}

		std::string ToString() const override
		{
			std::stringstream ss;
			ss << "KeyReleasedEvent: " << mKeyCode;
			return ss.str();
		}

		EVENT_CLASS_TYPE(KeyReleased)
	};
}