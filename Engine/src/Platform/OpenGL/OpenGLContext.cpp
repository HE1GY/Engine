//
// Created by oleh on 09.01.23.
//
#include "OpenGLContext.h"
#include "Engine/Core/Core.h"

#include "glad/glad.h"
#include "GLFW/glfw3.h"

namespace Engine
{

	OpenGLContext::OpenGLContext(GLFWwindow* window_handler)
			:m_window_handler{ window_handler }
	{
	}
	void OpenGLContext::Init()
	{
		PROFILER_FUNCTION();

		glfwMakeContextCurrent(m_window_handler);
		int status = gladLoadGLLoader((GLADloadproc)glfwGetProcAddress);
		ASSERT(status);

		CORE_INFO(" VENDOR: {0}", (const char*)glGetString(GL_VENDOR));
		CORE_INFO(" RENDERER: {0}", (const char*)glGetString(GL_RENDERER));
		CORE_INFO(" VERSION: {0}", (const char*)glGetString(GL_VERSION));
	};
	void OpenGLContext::SwapBuffer()
	{
		PROFILER_FUNCTION();

		glfwSwapBuffers(m_window_handler);
	};
} // namespace Engine