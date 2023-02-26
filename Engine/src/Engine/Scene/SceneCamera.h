#pragma once

#include "Engine/Renderer/Camera.h"

namespace Engine
{
	class SceneCamera : public Camera
	{
	public:
		SceneCamera() = default;
		virtual ~SceneCamera() override = default;

		void SetViewport(uint32_t width, uint32_t height)
		{
			m_aspect_ration = (float)width / (float)height;
			CalculateProjection();
		}

		float GetAspectRatio() const
		{
			return m_aspect_ration;
		}

		ProjectionType GetProjectionType() const
		{
			return m_projection_type;
		}

		float GetOrthographicSize() const
		{
			return m_ortho_size;
		}

		float GetOrthographicFarClip() const
		{
			return m_far_ortho;
		}

		float GetOrthographicNearClip() const
		{
			return m_near_ortho;
		}

		void SetOrthographicSize(float size)
		{
			m_ortho_size = size;
			CalculateProjection();
		}
		void SetOrthographicNearClip(float near)
		{
			m_near_ortho = near;
			CalculateProjection();
		}
		void SetOrthographicFarClip(float far)
		{
			m_far_ortho = far;
			CalculateProjection();
		}

		float GetPerspectiveFov() const
		{
			return m_fov;
		}

		float GetPerspectiveFarClip() const
		{
			return m_far_persp;
		}

		float GetPerspectiveNearClip() const
		{
			return m_near_persp;
		}

		void SetPerspectiveFov(float fov)
		{
			m_fov = fov;
			CalculateProjection();
		}
		void SetPerspectiveNearClip(float near)
		{
			m_near_persp = near;
			CalculateProjection();
		}
		void SetPerspectiveFarClip(float far)
		{
			m_far_persp = far;
			CalculateProjection();
		}
	};
}
