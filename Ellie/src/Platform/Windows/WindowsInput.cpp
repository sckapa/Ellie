#include "eepch.h"
#include "Ellie/Core/Input.h"
#include "Ellie/Core/Application.h"
#include "GLFW/glfw3.h"


namespace Ellie{

	bool Input::IsKeyPressed(int keycode)
	{
		auto window = static_cast<GLFWwindow*>(Application::Get().GetWindow().GetNativeWindow());
		int status = glfwGetKey(window, keycode);
		return status == GLFW_PRESS || status == GLFW_REPEAT;
	}

	bool Input::IsMouseButtonPressed(int button)
	{
		auto window = static_cast<GLFWwindow*>(Application::Get().GetWindow().GetNativeWindow());
		int status = glfwGetMouseButton(window, button);
		return status == GLFW_PRESS;
	}

	float Input::GetMouseX()
	{
		auto [x, y] = GetMousePosition();
		return x;
	}

	float Input::GetMouseY()
	{
		auto [x, y] = GetMousePosition();
		return y;
	}

	std::pair<float, float> Input::GetMousePosition()
	{
		auto window = static_cast<GLFWwindow*>(Application::Get().GetWindow().GetNativeWindow());
		double xPos, yPos;
		glfwGetCursorPos(window, &xPos, &yPos);
		return { xPos,yPos };
	}

}
