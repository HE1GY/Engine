#include <glm/ext/matrix_clip_space.hpp>
#include "pch.h"

#include "Camera.h"

namespace Engine
{
	void Camera::SetProjectionType(ProjectionType type)
	{
		m_projection_type = type;
		CalculateProjection();
	}
	void Camera::SetOrthographic(float aspect_ration, float size, float near, float far)
	{
		m_aspect_ration = aspect_ration;
		m_ortho_size = size;
		m_near_ortho = near;
		m_far_ortho = far;
		CalculateProjection();
	}
	void Camera::SetPerspective(float aspect_ration, float fov, float near, float far)
	{
		m_aspect_ration = aspect_ration;
		m_fov = fov;
		m_near_persp = near;
		m_far_persp = far;
		CalculateProjection();
	}
	void Camera::CalculateProjection()
	{
		if (m_projection_type == ProjectionType::Orthographic)
		{
			m_ortho_bound[0].x = -m_aspect_ration * 0.5f * m_ortho_size;
			m_ortho_bound[0].y = 0.5 * m_ortho_size;
			m_ortho_bound[1].x = m_aspect_ration * 0.5f * m_ortho_size;
			m_ortho_bound[1].y = -0.5 * m_ortho_size;

			m_projection = glm::ortho(m_ortho_bound[0].x, m_ortho_bound[1].x, m_ortho_bound[1].y, m_ortho_bound[0].y,
					m_near_ortho, m_far_ortho);
		}
		else
		{
			m_projection = glm::perspective(m_fov, m_aspect_ration, m_near_persp, m_far_persp);
		};
	}

}