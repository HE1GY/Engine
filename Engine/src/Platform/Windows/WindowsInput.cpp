#include "pch.h"

#include "Engine/Core/Application.h"
#include "GLFW/glfw3.h"
#include "WindowsInput.h"

namespace Engine
{
	Input* Input::s_instance = new WindowsInput();

	std::pair<float, float> Engine::WindowsInput::GetMousePosImpl()
	{
		GLFWwindow* window = static_cast<GLFWwindow*>(Application::Get()->GetWindow()->get_native_window());

		double x, y;
		glfwGetCursorPos(window, &x, &y);
		return { x, y };
	}
	bool Engine::WindowsInput::IsMouseButtonPressImpl(int button)
	{
		GLFWwindow* window = static_cast<GLFWwindow*>(Application::Get()->GetWindow()->get_native_window());

		bool press = glfwGetMouseButton(window, button);
		return press;
	}
	bool Engine::WindowsInput::IsKeyPressImpl(int key_code)
	{
		GLFWwindow* window = static_cast<GLFWwindow*>(Application::Get()->GetWindow()->get_native_window());

		bool press = glfwGetKey(window, key_code);
		return press;
	}
} // namespace Engine
