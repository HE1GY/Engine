#pragma once

#include "Engine/Renderer/Camera.h"

namespace Engine
{
	class SceneCamera : public Camera
	{
	public:
		enum class ProjectionType
		{
			Perspective, Orthographic
		};

		SceneCamera();
		virtual ~SceneCamera() override = default;

		void set_projection_type(ProjectionType type)
		{
			m_projection_type = type;
		}

		void set_viewport(uint32_t width, uint32_t height);
		void set_orthographic(float size, float near_clip, float far_clip);

		void set_orthographic_size(float orthographic_size)
		{
			m_orthographic_size = orthographic_size;
			RecalculateProjection();
		}

		void set_orthographic_near_clip(float value)
		{
			m_orthographic_near = value;
			RecalculateProjection();
		}

		void set_orthographic_far_clip(float value)
		{
			m_orthographic_far = value;
			RecalculateProjection();
		}

		ProjectionType get_projection_type() const
		{
			return m_projection_type;
		}

		float get_orthographic_size() const
		{
			return m_orthographic_size;
		}

		float get_orthographic_far_clip() const
		{
			return m_orthographic_far;
		}
		float get_orthographic_near_clip() const
		{
			return m_orthographic_near;
		}

		//perspective
		void set_perspective(float FOV, float near_clip, float far_clip);

		void set_perspective_FOV(float FOV)
		{
			m_perspective_FOV = FOV;
			RecalculateProjection();
		}

		void set_perspective_near_clip(float value)
		{
			m_perspective_near = value;
			RecalculateProjection();
		}

		void set_perspective_far_clip(float value)
		{
			m_perspective_far = value;
			RecalculateProjection();
		}

		float get_perspective_FOV() const
		{
			return m_perspective_FOV;
		}

		float get_perspective_far_clip() const
		{
			return m_perspective_far;
		}
		float get_perspective_near_clip() const
		{
			return m_perspective_near;
		}

	private:
		void RecalculateProjection();

		ProjectionType m_projection_type;

		float m_orthographic_size{ 10 };
		float m_orthographic_near{ -1 };
		float m_orthographic_far{ 1 };

		float m_perspective_FOV{ glm::radians(45.0f) };
		float m_perspective_near{ -0.01f };
		float m_perspective_far{ 1000.0f };
		float m_aspect_ration;
	};
}
