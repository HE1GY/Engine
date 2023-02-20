#include "pch.h"

#include "SceneCamera.h"
#include "glm/gtc/matrix_transform.hpp"

namespace Engine
{
	void SceneCamera::SetViewport(uint32_t width, uint32_t height)
	{
		m_aspect_ration = (float)width / (float)height;
		RecalculateProjection();
	}
	void SceneCamera::SetOrthographic(float size, float near_clip, float far_clip)
	{
		m_orthographic_size = size;
		m_orthographic_near = near_clip;
		m_orthographic_far = far_clip;
	}
	void SceneCamera::RecalculateProjection()
	{
		if (m_projection_type == ProjectionType::Orthographic)
		{
			m_orthographic_bound[0].x = -0.5f * m_aspect_ration * m_orthographic_size;
			m_orthographic_bound[0].y = 0.5f * m_orthographic_size;
			m_orthographic_bound[1].x = 0.5f * m_aspect_ration * m_orthographic_size;
			m_orthographic_bound[1].y = -0.5f * m_orthographic_size;

			m_projection = glm::ortho(m_orthographic_bound[0].x, m_orthographic_bound[1].x, m_orthographic_bound[1].y,
					m_orthographic_bound[0].y, m_orthographic_near,
					m_orthographic_far);
		}
		else
		{
			m_projection = glm::perspective(m_perspective_FOV, m_aspect_ration, m_perspective_near, m_perspective_far);
		}
	}
	void SceneCamera::set_perspective(float FOV, float near_clip, float far_clip)
	{
		m_perspective_FOV = FOV;
		m_perspective_near = near_clip;
		m_perspective_far = far_clip;
	}
}