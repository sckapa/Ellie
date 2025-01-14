#pragma once

#include "Ellie/Input.h"


namespace Ellie{

	class WindowsInput : public Input
	{
		virtual bool IsKeyPressedImpl(int keycode) override;

		virtual bool IsMouseButtonPressedImpl(int button) override;
		virtual float GetMouseXImpl() override;
		virtual float GetMouseYImpl() override;
		virtual std::pair<float, float> GetMouseImpl() override;
	};

}

