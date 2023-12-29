#pragma once

namespace Zital
{

	typedef enum class MouseCode : uint16_t
	{
		// From glfw3.h
		Button0				= 0,
		Button1				= 1,
		Button2				= 2,
		Button3				= 3,
		Button4				= 4,
		Button5				= 5,
		Button6				= 6,
		Button7				= 7,

		ButtonLast			= Button7,
		ButtonLeft			= Button0,
		ButtonRight			= Button1,
		ButtonMiddle		= Button2
	} Mouse;

	inline std::ostream& operator<<(std::ostream& os, MouseCode _code)
	{
		os << static_cast<int32_t>(_code);
		return os;
	}

}

//from glfw3.h
#define ZT_MOUSE_BUTTON_0         ::Zital::Mouse::Button0
#define ZT_MOUSE_BUTTON_1         ::Zital::Mouse::Button1
#define ZT_MOUSE_BUTTON_2         ::Zital::Mouse::Button2
#define ZT_MOUSE_BUTTON_3         ::Zital::Mouse::Button3
#define ZT_MOUSE_BUTTON_4         ::Zital::Mouse::Button4
#define ZT_MOUSE_BUTTON_5         ::Zital::Mouse::Button5
#define ZT_MOUSE_BUTTON_6         ::Zital::Mouse::Button6
#define ZT_MOUSE_BUTTON_7         ::Zital::Mouse::Button7
#define ZT_MOUSE_BUTTON_LAST      ::Zital::Mouse::ButtonLast
#define ZT_MOUSE_BUTTON_LEFT      ::Zital::Mouse::ButtonLeft
#define ZT_MOUSE_BUTTON_RIGHT     ::Zital::Mouse::ButtonRight
#define ZT_MOUSE_BUTTON_MIDDLE    ::Zital::Mouse::ButtonMiddle