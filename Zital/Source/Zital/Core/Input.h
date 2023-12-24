#pragma once

#include "Zital/Core/Core.h"
#include "Zital/Core/KeyCodes.h"
#include "Zital/Core/MouseButtonCodes.h"

namespace Zital
{

	class Input
	{
	public:
		static bool IsKeyPressed(int _keyCode);

		static bool IsMouseButtonPressed(int _button);
		static std::pair<float, float> GetMousePosition();
		static float GetMouseX();
		static float GetMouseY();
	};

}