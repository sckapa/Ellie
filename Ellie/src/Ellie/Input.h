#pragma once

#include "Core.h"

namespace Ellie {

	class ELLIE_API Input
	{
	public:
		static bool IsKeyPressed(int keycode) { return s_Input->IsKeyPressedImpl(keycode); }

		static bool IsMouseButtonPressed(int button) { return s_Input->IsMouseButtonPressedImpl(button); }
		static float GetMouseX() { return s_Input->GetMouseXImpl(); }
		static float GetMouseY() { return s_Input->GetMouseYImpl(); }
		static std::pair<float, float> GetMousePosition() { return s_Input->GetMouseImpl(); }

	protected:
		virtual bool IsKeyPressedImpl(int keycode) = 0;

		virtual bool IsMouseButtonPressedImpl(int button) = 0;
		virtual float GetMouseXImpl() = 0;
		virtual float GetMouseYImpl() = 0;
		virtual std::pair<float, float> GetMouseImpl() = 0;
	private:
		static Input* s_Input;
	};

}