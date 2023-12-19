#pragma once

#include "Zital/Core/Core.h"

namespace Zital
{

	class ZITAL_API Input
	{
	public:
		inline static bool IsKeyPressed(int _keyCode) { return sInstance->IsKeyPressedImpl(_keyCode); }

		inline static bool IsMouseButtonPressed(int _button) { return sInstance->IsMouseButtonPressedImpl(_button); }
		inline static std::pair<float, float> GetMousePosition() { return sInstance->GetMousePositionImpl(); }
		inline static float GetMouseX() { return sInstance->GetMouseXImpl(); }
		inline static float GetMouseY() { return sInstance->GetMouseYImpl(); }

	protected:
		virtual bool IsKeyPressedImpl(int _keyCode) = 0;

		virtual bool IsMouseButtonPressedImpl(int _button) = 0;
		virtual std::pair<float, float> GetMousePositionImpl() = 0;
		virtual float GetMouseXImpl() = 0;
		virtual float GetMouseYImpl() = 0;

	private:
		static Scope<Input> sInstance;
	};

}