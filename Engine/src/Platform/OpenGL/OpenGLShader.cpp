#include "pch.h"

#include "Engine/Core.h"
#include "OpenGLShader.h"
#include "glad/glad.h"
#include <glm/gtc/type_ptr.hpp>

namespace Engine
{
	OpenGLShader::OpenGLShader(std::string vertex_src, std::string fragment_src)
	{
		// Read our shaders into the appropriate buffers
		std::string vertexSource = vertex_src;     // Get source code for vertex shader.
		std::string fragmentSource = fragment_src; // Get source code for fragment shader.

		// Create an empty vertex shader handle
		GLuint vertexShader = glCreateShader(GL_VERTEX_SHADER);

		// Send the vertex shader source code to GL
		// Note that std::string's .c_str is NULL character terminated.
		const GLchar* source = (const GLchar*)vertexSource.c_str();
		glShaderSource(vertexShader, 1, &source, 0);

		// Compile the vertex shader
		glCompileShader(vertexShader);

		GLint isCompiled = 0;
		glGetShaderiv(vertexShader, GL_COMPILE_STATUS, &isCompiled);
		if (isCompiled == GL_FALSE)
		{
			GLint maxLength = 0;
			glGetShaderiv(vertexShader, GL_INFO_LOG_LENGTH, &maxLength);

			// The maxLength includes the NULL character
			std::vector<GLchar> infoLog(maxLength);
			glGetShaderInfoLog(vertexShader, maxLength, &maxLength, &infoLog[0]);

			// We don't need the shader anymore.
			glDeleteShader(vertexShader);

			CORE_ERROR("Vertex Shader doesn't compile");
			CORE_ERROR(infoLog.data());
			ASSERT(false);
		}

		// Create an empty fragment shader handle
		GLuint fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);

		// Send the fragment shader source code to GL
		// Note that std::string's .c_str is NULL character terminated.
		source = (const GLchar*)fragmentSource.c_str();
		glShaderSource(fragmentShader, 1, &source, 0);

		// Compile the fragment shader
		glCompileShader(fragmentShader);

		glGetShaderiv(fragmentShader, GL_COMPILE_STATUS, &isCompiled);
		if (isCompiled == GL_FALSE)
		{
			GLint maxLength = 0;
			glGetShaderiv(fragmentShader, GL_INFO_LOG_LENGTH, &maxLength);

			// The maxLength includes the NULL character
			std::vector<GLchar> infoLog(maxLength);
			glGetShaderInfoLog(fragmentShader, maxLength, &maxLength, &infoLog[0]);

			// We don't need the shader anymore.
			glDeleteShader(fragmentShader);
			// Either of them. Don't leak shaders.
			glDeleteShader(vertexShader);

			CORE_ERROR("Fragment Shader doesn't compiled");
			CORE_ERROR(infoLog.data());
			ASSERT(false);
		}

		// Vertex and fragment shaders are successfully compiled.
		// Now time to link them together into a program.
		// Get a program object.
		m_renderer_id = glCreateProgram();

		// Attach our shaders to our program
		glAttachShader(m_renderer_id, vertexShader);
		glAttachShader(m_renderer_id, fragmentShader);

		// Link our program
		glLinkProgram(m_renderer_id);

		// Note the different functions here: glGetProgram* instead of glGetShader*.
		GLint isLinked = 0;
		glGetProgramiv(m_renderer_id, GL_LINK_STATUS, (int*)&isLinked);
		if (isLinked == GL_FALSE)
		{
			GLint maxLength = 0;
			glGetProgramiv(m_renderer_id, GL_INFO_LOG_LENGTH, &maxLength);

			// The maxLength includes the NULL character
			std::vector<GLchar> infoLog(maxLength);
			glGetProgramInfoLog(m_renderer_id, maxLength, &maxLength, &infoLog[0]);

			// We don't need the program anymore.
			glDeleteProgram(m_renderer_id);
			// Don't leak shaders either.
			glDeleteShader(vertexShader);
			glDeleteShader(fragmentShader);

			CORE_ERROR("Shader program doesn't linked");
			CORE_ERROR(infoLog.data());
			ASSERT(false);
		}

		// Always detach shaders after a successful link.
		glDetachShader(m_renderer_id, vertexShader);
		glDetachShader(m_renderer_id, fragmentShader);
	}

	void OpenGLShader::UploadUniform(const std::string& name, float value)
	{
		ASSERT(m_renderer_id);

		GLint location = glGetUniformLocation(m_renderer_id, name.c_str());
		glUniform1f(location, value);
	}

	void OpenGLShader::UploadUniform(const std::string& name, const glm::mat4& value)
	{
		ASSERT(m_renderer_id);

		GLint location = glGetUniformLocation(m_renderer_id, name.c_str());
		glUniformMatrix4fv(location, 1, GL_FALSE, glm::value_ptr(value));
	}

	void OpenGLShader::UploadUniform(const std::string& name, const glm::vec4& value)
	{
		ASSERT(m_renderer_id);
		GLint location = glGetUniformLocation(m_renderer_id, name.c_str());
		glUniform4f(location, value.r, value.g, value.b, value.a);
	}

	void OpenGLShader::Bind() const
	{
		glUseProgram(m_renderer_id);
	}
} // namespace Engine