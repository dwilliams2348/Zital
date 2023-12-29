#pragma once

namespace Zital
{

	typedef enum class KeyCode : uint16_t
	{
		Space				= 32,
		Apostrophe			= 39,  /* ' */
		Comma				= 44,  /* , */
		Minus				= 45,  /* - */
		Period				= 46,  /* . */
		Slash				= 47,  /* / */
								   
		D0					= 48,  /* 0 */
		D1					= 49,  /* 1 */
		D2					= 50,  /* 2 */
		D3					= 51,  /* 3 */
		D4					= 52,  /* 4 */
		D5					= 53,  /* 5 */
		D6					= 54,  /* 6 */
		D7					= 55,  /* 7 */
		D8					= 56,  /* 8 */
		D9					= 57,  /* 9 */
								   
		Semicolon			= 59,  /* ; */
		Equal				= 61,  /* = */

		A					= 65,
		B					= 66,
		C					= 67,
		D					= 68,
		E					= 69,
		F					= 70,
		G					= 71,
		H					= 72,
		I					= 73,
		J					= 74,
		K					= 75,
		L					= 76,
		M					= 77,
		N					= 78,
		O					= 79,
		P					= 80,
		Q					= 81,
		R					= 82,
		S					= 83,
		T					= 84,
		U					= 85,
		V					= 86,
		W					= 87,
		X					= 88,
		Y					= 89,
		Z					= 90,

		LeftBracket			= 91,  /* [ */
		Backslash			= 92,  /* \ */
		RightBracket		= 93,  /* ] */
		GraveAccent			= 96,  /* ` */

		World1				= 161, /* non-US #1 */
		World2				= 162, /* non-US #2 */

		/* Function keys */
		Escape				= 256,
		Enter				= 257,
		Tab					= 258,
		Backspace			= 259,
		Insert				= 260,
		Delete				= 261,
		Right				= 262,
		Left				= 263,
		Down				= 264,
		Up					= 265,
		PageUp				= 266,
		PageDown			= 267,
		Home				= 268,
		End					= 269,
		CapsLock			= 280,
		ScrollLock			= 281,
		NumLock				= 282,
		PrintScreen			= 283,
		Pause				= 284,
		F1					= 290,
		F2					= 291,
		F3					= 292,
		F4					= 293,
		F5					= 294,
		F6					= 295,
		F7					= 296,
		F8					= 297,
		F9					= 298,
		F10					= 299,
		F11					= 300,
		F12					= 301,
		F13					= 302,
		F14					= 303,
		F15					= 304,
		F16					= 305,
		F17					= 306,
		F18					= 307,
		F19					= 308,
		F20					= 309,
		F21					= 310,
		F22					= 311,
		F23					= 312,
		F24					= 313,
		F25					= 314,

		/* Keypad */
		KP0					= 320,
		KP1					= 321,
		KP2					= 322,
		KP3					= 323,
		KP4					= 324,
		KP5					= 325,
		KP6					= 326,
		KP7					= 327,
		KP8					= 328,
		KP9					= 329,
		KPDecimal			= 330,
		KPDivide			= 331,
		KPMultiply			= 332,
		KPSubtract			= 333,
		KPAdd				= 334,
		KPEnter				= 335,
		KPEqual				= 336,

		LeftShift			= 340,
		LeftControl			= 341,
		LeftAlt				= 342,
		LeftSuper			= 343,
		RightShift			= 344,
		RightControl		= 345,
		RightAlt			= 346,
		RightSuper			= 347,
		Menu				= 348
	} Key;

	inline std::ostream& operator<<(std::ostream& os, KeyCode _key)
	{
		os << static_cast<int32_t>(_key);
		return os;
	}
}

//From glfw3.h
// From glfw3.h
#define ZT_KEY_SPACE           ::Zital::Key::Space
#define ZT_KEY_APOSTROPHE      ::Zital::Key::Apostrophe    /* ' */
#define ZT_KEY_COMMA           ::Zital::Key::Comma         /* , */
#define ZT_KEY_MINUS           ::Zital::Key::Minus         /* - */
#define ZT_KEY_PERIOD          ::Zital::Key::Period        /* . */
#define ZT_KEY_SLASH           ::Zital::Key::Slash         /* / */
#define ZT_KEY_0               ::Zital::Key::D0
#define ZT_KEY_1               ::Zital::Key::D1
#define ZT_KEY_2               ::Zital::Key::D2
#define ZT_KEY_3               ::Zital::Key::D3
#define ZT_KEY_4               ::Zital::Key::D4
#define ZT_KEY_5               ::Zital::Key::D5
#define ZT_KEY_6               ::Zital::Key::D6
#define ZT_KEY_7               ::Zital::Key::D7
#define ZT_KEY_8               ::Zital::Key::D8
#define ZT_KEY_9               ::Zital::Key::D9
#define ZT_KEY_SEMICOLON       ::Zital::Key::Semicolon     /* ; */
#define ZT_KEY_EQUAL           ::Zital::Key::Equal         /* = */
#define ZT_KEY_A               ::Zital::Key::A
#define ZT_KEY_B               ::Zital::Key::B
#define ZT_KEY_C               ::Zital::Key::C
#define ZT_KEY_D               ::Zital::Key::D
#define ZT_KEY_E               ::Zital::Key::E
#define ZT_KEY_F               ::Zital::Key::F
#define ZT_KEY_G               ::Zital::Key::G
#define ZT_KEY_H               ::Zital::Key::H
#define ZT_KEY_I               ::Zital::Key::I
#define ZT_KEY_J               ::Zital::Key::J
#define ZT_KEY_K               ::Zital::Key::K
#define ZT_KEY_L               ::Zital::Key::L
#define ZT_KEY_M               ::Zital::Key::M
#define ZT_KEY_N               ::Zital::Key::N
#define ZT_KEY_O               ::Zital::Key::O
#define ZT_KEY_P               ::Zital::Key::P
#define ZT_KEY_Q               ::Zital::Key::Q
#define ZT_KEY_R               ::Zital::Key::R
#define ZT_KEY_S               ::Zital::Key::S
#define ZT_KEY_T               ::Zital::Key::T
#define ZT_KEY_U               ::Zital::Key::U
#define ZT_KEY_V               ::Zital::Key::V
#define ZT_KEY_W               ::Zital::Key::W
#define ZT_KEY_X               ::Zital::Key::X
#define ZT_KEY_Y               ::Zital::Key::Y
#define ZT_KEY_Z               ::Zital::Key::Z
#define ZT_KEY_LEFT_BRACKET    ::Zital::Key::LeftBracket   /* [ */
#define ZT_KEY_BACKSLASH       ::Zital::Key::Backslash     /* \ */
#define ZT_KEY_RIGHT_BRACKET   ::Zital::Key::RightBracket  /* ] */
#define ZT_KEY_GRAVE_ACCENT    ::Zital::Key::GraveAccent   /* ` */
#define ZT_KEY_WORLD_1         ::Zital::Key::World1        /* non-US #1 */
#define ZT_KEY_WORLD_2         ::Zital::Key::World2        /* non-US #2 */
								 
/* Function keys */				 
#define ZT_KEY_ESCAPE          ::Zital::Key::Escape
#define ZT_KEY_ENTER           ::Zital::Key::Enter
#define ZT_KEY_TAB             ::Zital::Key::Tab
#define ZT_KEY_BACKSPACE       ::Zital::Key::Backspace
#define ZT_KEY_INSERT          ::Zital::Key::Insert
#define ZT_KEY_DELETE          ::Zital::Key::Delete
#define ZT_KEY_RIGHT           ::Zital::Key::Right
#define ZT_KEY_LEFT            ::Zital::Key::Left
#define ZT_KEY_DOWN            ::Zital::Key::Down
#define ZT_KEY_UP              ::Zital::Key::Up
#define ZT_KEY_PAGE_UP         ::Zital::Key::PageUp
#define ZT_KEY_PAGE_DOWN       ::Zital::Key::PageDown
#define ZT_KEY_HOME            ::Zital::Key::Home
#define ZT_KEY_END             ::Zital::Key::End
#define ZT_KEY_CAPS_LOCK       ::Zital::Key::CapsLock
#define ZT_KEY_SCROLL_LOCK     ::Zital::Key::ScrollLock
#define ZT_KEY_NUM_LOCK        ::Zital::Key::NumLock
#define ZT_KEY_PRINT_SCREEN    ::Zital::Key::PrintScreen
#define ZT_KEY_PAUSE           ::Zital::Key::Pause
#define ZT_KEY_F1              ::Zital::Key::F1
#define ZT_KEY_F2              ::Zital::Key::F2
#define ZT_KEY_F3              ::Zital::Key::F3
#define ZT_KEY_F4              ::Zital::Key::F4
#define ZT_KEY_F5              ::Zital::Key::F5
#define ZT_KEY_F6              ::Zital::Key::F6
#define ZT_KEY_F7              ::Zital::Key::F7
#define ZT_KEY_F8              ::Zital::Key::F8
#define ZT_KEY_F9              ::Zital::Key::F9
#define ZT_KEY_F10             ::Zital::Key::F10
#define ZT_KEY_F11             ::Zital::Key::F11
#define ZT_KEY_F12             ::Zital::Key::F12
#define ZT_KEY_F13             ::Zital::Key::F13
#define ZT_KEY_F14             ::Zital::Key::F14
#define ZT_KEY_F15             ::Zital::Key::F15
#define ZT_KEY_F16             ::Zital::Key::F16
#define ZT_KEY_F17             ::Zital::Key::F17
#define ZT_KEY_F18             ::Zital::Key::F18
#define ZT_KEY_F19             ::Zital::Key::F19
#define ZT_KEY_F20             ::Zital::Key::F20
#define ZT_KEY_F21             ::Zital::Key::F21
#define ZT_KEY_F22             ::Zital::Key::F22
#define ZT_KEY_F23             ::Zital::Key::F23
#define ZT_KEY_F24             ::Zital::Key::F24
#define ZT_KEY_F25             ::Zital::Key::F25
								 
/* Keypad */					 
#define ZT_KEY_KP_0            ::Zital::Key::KP0
#define ZT_KEY_KP_1            ::Zital::Key::KP1
#define ZT_KEY_KP_2            ::Zital::Key::KP2
#define ZT_KEY_KP_3            ::Zital::Key::KP3
#define ZT_KEY_KP_4            ::Zital::Key::KP4
#define ZT_KEY_KP_5            ::Zital::Key::KP5
#define ZT_KEY_KP_6            ::Zital::Key::KP6
#define ZT_KEY_KP_7            ::Zital::Key::KP7
#define ZT_KEY_KP_8            ::Zital::Key::KP8
#define ZT_KEY_KP_9            ::Zital::Key::KP9
#define ZT_KEY_KP_DECIMAL      ::Zital::Key::KPDecimal
#define ZT_KEY_KP_DIVIDE       ::Zital::Key::KPDivide
#define ZT_KEY_KP_MULTIPLY     ::Zital::Key::KPMultiply
#define ZT_KEY_KP_SUBTRACT     ::Zital::Key::KPSubtract
#define ZT_KEY_KP_ADD          ::Zital::Key::KPAdd
#define ZT_KEY_KP_ENTER        ::Zital::Key::KPEnter
#define ZT_KEY_KP_EQUAL        ::Zital::Key::KPEqual
								
#define ZT_KEY_LEFT_SHIFT      ::Zital::Key::LeftShift
#define ZT_KEY_LEFT_CONTROL    ::Zital::Key::LeftControl
#define ZT_KEY_LEFT_ALT        ::Zital::Key::LeftAlt
#define ZT_KEY_LEFT_SUPER      ::Zital::Key::LeftSuper
#define ZT_KEY_RIGHT_SHIFT     ::Zital::Key::RightShift
#define ZT_KEY_RIGHT_CONTROL   ::Zital::Key::RightControl
#define ZT_KEY_RIGHT_ALT       ::Zital::Key::RightAlt
#define ZT_KEY_RIGHT_SUPER     ::Zital::Key::RightSuper
#define ZT_KEY_MENU            ::Zital::Key::Menu