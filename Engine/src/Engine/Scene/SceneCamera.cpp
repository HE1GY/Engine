#include "pch.h"

#include "SceneCamera.h"
#include "glm/gtc/matrix_transform.hpp"

namespace Engine
{

	SceneCamera::SceneCamera()
	{

	}
	void SceneCamera::set_viewport(uint32_t width, uint32_t height)
	{
		m_aspect_ration = (float)width / (float)height;
		RecalculateProjection();
	}
	void SceneCamera::set_orthographic(float size, float near_clip, float far_clip)
	{
		float m_orthographic_size = size;
		float m_orthographic_near = near_clip;
		float m_orthographic_far = far_clip;
	}
	void SceneCamera::RecalculateProjection()
	{
		float ortho_left = -0.5f * m_aspect_ration * m_orthographic_size;
		float ortho_right = 0.5f * m_aspect_ration * m_orthographic_size;
		float ortho_bottom = -0.5f * m_orthographic_size;
		float ortho_top = 0.5f * m_orthographic_size;

		m_projection = glm::ortho(ortho_left, ortho_right, ortho_bottom, ortho_top, m_orthographic_near,
				m_orthographic_far);
	}
}