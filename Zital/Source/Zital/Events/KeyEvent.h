#pragma once

#include "Zital/Events/Event.h"
#include "Zital/Core/Input.h"

namespace Zital
{

	class KeyEvent : public Event
	{
	public:
		inline KeyCode GetKeyCode() const { return mKeyCode; }

		EVENT_CLASS_CATEGORY(EventCategoryKeyboard | EventCategoryInput)

	protected:
		KeyEvent(KeyCode _keycode)
			: mKeyCode(_keycode) {}

		KeyCode mKeyCode;
	};

	class KeyPressedEvent : public KeyEvent
	{
	public:
		KeyPressedEvent(KeyCode _keycode, int _repeatCount)
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

	class KeyReleasedEvent : public KeyEvent
	{
	public:
		KeyReleasedEvent(KeyCode _keycode)
			: KeyEvent(_keycode) {}

		std::string ToString() const override
		{
			std::stringstream ss;
			ss << "KeyReleasedEvent: " << mKeyCode;
			return ss.str();
		}

		EVENT_CLASS_TYPE(KeyReleased)
	};

	class KeyTypedEvent : public KeyEvent
	{
	public:
		KeyTypedEvent(KeyCode _keycode)
			: KeyEvent(_keycode) {}

		std::string ToString() const override
		{
			std::stringstream ss;
			ss << "KeyTypedEvent: " << mKeyCode ;
			return ss.str();
		}

		EVENT_CLASS_TYPE(KeyTyped)
	};
}