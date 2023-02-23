#include "OpenGLContext.h"
#include "Engine/Core/Core.h"

#include "glad/glad.h"
#include "GLFW/glfw3.h"

namespace Engine
{
	static void OpenGLErrorCallback(GLenum source, GLenum type, GLuint id, GLenum severity, GLsizei length,
			const GLchar* message, const void* userParam)
	{
		std::stringstream ss;
		ss << "OpenGL" << std::endl;
		ss << "Debug message (" << id << "): " << message << std::endl;

		switch (source)
		{
		case GL_DEBUG_SOURCE_API:
			ss << "Source: API";
			break;
		case GL_DEBUG_SOURCE_WINDOW_SYSTEM:
			ss << "Source: Window System";
			break;
		case GL_DEBUG_SOURCE_SHADER_COMPILER:
			ss << "Source: Shader Compiler";
			break;
		case GL_DEBUG_SOURCE_THIRD_PARTY:
			ss << "Source: Third Party";
			break;
		case GL_DEBUG_SOURCE_APPLICATION:
			ss << "Source: Application";
			break;
		case GL_DEBUG_SOURCE_OTHER:
			ss << "Source: Other";
			break;
		}
		ss << std::endl;

		switch (severity)
		{
		case GL_DEBUG_SEVERITY_HIGH:
			ASSERT(false);
			ERROR(ss.str());
			break;
		case GL_DEBUG_SEVERITY_MEDIUM:
			WARN(ss.str());
			break;
		case GL_DEBUG_SEVERITY_LOW:
			INFO(ss.str());
			break;
/*case GL_DEBUG_SEVERITY_NOTIFICATION:
	TRACE(ss.str());
	break;*/
		}
	}

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

		glEnable(GL_DEBUG_OUTPUT);
		glEnable(GL_DEBUG_OUTPUT_SYNCHRONOUS);
		glDebugMessageCallback(OpenGLErrorCallback, nullptr);

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