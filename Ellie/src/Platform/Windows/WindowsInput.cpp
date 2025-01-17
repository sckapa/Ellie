#include "eepch.h"
#include "WindowsInput.h"
#include "Ellie/Core/Application.h"
#include "GLFW/glfw3.h"


namespace Ellie{

	Input* Input::s_Input = new WindowsInput();

	bool WindowsInput::IsKeyPressedImpl(int keycode)
	{
		auto window = static_cast<GLFWwindow*>(Application::Get().GetWindow().GetNativeWindow());
		int status = glfwGetKey(window, keycode);
		return status == GLFW_PRESS || status == GLFW_REPEAT;
	}

	bool WindowsInput::IsMouseButtonPressedImpl(int button)
	{
		auto window = static_cast<GLFWwindow*>(Application::Get().GetWindow().GetNativeWindow());
		int status = glfwGetMouseButton(window, button);
		return status == GLFW_PRESS;
	}

	float WindowsInput::GetMouseXImpl()
	{
		auto [x, y] = GetMouseImpl();
		return x;
	}

	float WindowsInput::GetMouseYImpl()
	{
		auto [x, y] = GetMouseImpl();
		return y;
	}

	std::pair<float, float> WindowsInput::GetMouseImpl()
	{
		auto window = static_cast<GLFWwindow*>(Application::Get().GetWindow().GetNativeWindow());
		double xPos, yPos;
		glfwGetCursorPos(window, &xPos, &yPos);
		return { xPos,yPos };
	}

}
