#pragma once

#include "Zital/Input.h"

namespace Zital
{

	class WindowsInput : public Input
	{
	protected:
		virtual bool IsKeyPressedImpl(int _keyCode) override;

		virtual bool IsMouseButtonPressedImpl(int _button) override;
		virtual std::pair<float, float> GetMousePositionImpl() override;
		virtual float GetMouseXImpl() override;
		virtual float GetMouseYImpl() override;
	};

}