#include "pch.h"

#include "GLFW/glfw3.h"

#include "Engine/Core/Application.h"

#include "Engine/Core/Input.h"

namespace Engine
{
	std::pair<float, float> Input::GetMousePos()
	{
		GLFWwindow* window = static_cast<GLFWwindow*>(Application::get()->GetWindow()->GetNativeWindow());

		double x, y;
		glfwGetCursorPos(window, &x, &y);
		return { x, y };
	}
	bool Input::IsMouseButtonPress(int button)
	{
		GLFWwindow* window = static_cast<GLFWwindow*>(Application::get()->GetWindow()->GetNativeWindow());

		bool press = glfwGetMouseButton(window, button);
		return press;
	}
	bool Input::IsKeyPress(int key_code)
	{
		GLFWwindow* window = static_cast<GLFWwindow*>(Application::get()->GetWindow()->GetNativeWindow());

		bool press = glfwGetKey(window, key_code);
		return press;
	}

	bool Input::IsMouseButtonPress(MouseButtonCode button)
	{
		return IsMouseButtonPress((int)button);
	}
	bool Input::IsKeyPress(KeyCode key_code)
	{
		return IsKeyPress((int)key_code);
	}
} // namespace Engine
