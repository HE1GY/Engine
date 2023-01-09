#include "pch.h"

#include "WindowsInput.h"
#include "Engine/Application.h"
#include "GLFW/glfw3.h"

namespace Engine
{
Input *Input::s_instance = new WindowsInput();

std::pair<float, float> Engine::WindowsInput::GetMousePosImpl()
{
    GLFWwindow *window = static_cast<GLFWwindow *>(Application::Get()->GetWindow()->GetNativeWindow());

    double x, y;
    glfwGetCursorPos(window, &x, &y);
    return {x, y};
}
bool Engine::WindowsInput::IsMouseButtonPressImpl(int button)
{
    GLFWwindow *window = static_cast<GLFWwindow *>(Application::Get()->GetWindow()->GetNativeWindow());

    bool press = glfwGetMouseButton(window, button);
    return press;
}
bool Engine::WindowsInput::IsKeyPressImpl(int key_code)
{
    GLFWwindow *window = static_cast<GLFWwindow *>(Application::Get()->GetWindow()->GetNativeWindow());

    bool press = glfwGetKey(window, key_code);
    return press;
}
} // namespace Engine
