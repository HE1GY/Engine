#include "pch.h"

#include "OpenGLRendererAPI.h"
#include "glad/glad.h"

namespace Engine
{

	void OpenGLRendererAPI::SetClearColor(const glm::vec4& color)
	{
		glClearColor(color.r, color.g, color.b, color.a);
	}

	void OpenGLRendererAPI::DrawIndex(std::shared_ptr<VertexArray> vertex_array)
	{
		glDrawElements(GL_TRIANGLES, vertex_array->get_indexBuffer()->get_count(), GL_UNSIGNED_INT, nullptr);
	}
	void OpenGLRendererAPI::Clear()
	{
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	}
}

