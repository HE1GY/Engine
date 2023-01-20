#include "pch.h"

#include "OrthographicCamera.h"
#include "glm/gtc/matrix_transform.hpp"

namespace Engine
{
	OrthographicCamera::OrthographicCamera(float left, float right, float bottom, float top)
			:m_projection_matrix(glm::ortho(left, right, bottom, top, -1.0f, 1.0f)),
			 m_view_matrix(1), m_position{ 0, 0, 0 }, m_rotation{ 0 }
	{
		m_view_projection_matrix = m_projection_matrix * m_view_matrix;
	}
	void OrthographicCamera::RecalculateViewMatrix()
	{
		PROFILER_FUNCTION();

		glm::mat4 transform = glm::translate(glm::mat4(1.0f), m_position) *
				glm::rotate(glm::mat4(1.0f), glm::radians(m_rotation), glm::vec3(0, 0, 1));
		m_view_matrix = glm::inverse(transform);
		m_view_projection_matrix = m_projection_matrix * m_view_matrix;
	}
	void OrthographicCamera::set_projection(float left, float right, float bottom, float top)
	{
		PROFILER_FUNCTION();

		m_projection_matrix = glm::ortho(left, right, bottom, top, -1.0f, 1.0f);
		m_view_projection_matrix = m_projection_matrix * m_view_matrix;
	}

} // namespace Engine