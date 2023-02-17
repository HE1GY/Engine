#include "pch.h"

#include "Engine/Core/Core.h"
#include "OpenGLRendererAPI.h"
#include "glad/glad.h"

namespace Engine
{
	void OpenGLRendererAPI::Init()
	{
		PROFILER_FUNCTION();

		glEnable(GL_DEPTH_TEST);
		glEnable(GL_BLEND);
		glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	}
	void OpenGLRendererAPI::SetClearColor(const glm::vec4& color)
	{
		PROFILER_FUNCTION();

		glClearColor(color.r, color.g, color.b, color.a);
	}

	void OpenGLRendererAPI::DrawIndex(Ref<VertexArray> vertex_array, uint32_t index_count)
	{
		PROFILER_FUNCTION();
		vertex_array->Bind();
		uint32_t count = index_count;
		glDrawElements(GL_TRIANGLES, count, GL_UNSIGNED_INT, nullptr);
	}
	void OpenGLRendererAPI::Clear()
	{
		PROFILER_FUNCTION();
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	}
	void OpenGLRendererAPI::SetViewport(uint32_t x, uint32_t y, uint32_t width, uint32_t height)
	{
		PROFILER_FUNCTION();

		glViewport(x, y, width, height);
	}
} // namespace Engine
