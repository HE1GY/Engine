#pragma once

#include "Engine/Renderer/Camera.h"

namespace Engine
{
	class SceneCamera : public Camera
	{
	public:
		SceneCamera();
		virtual ~SceneCamera() override = default;

		void set_viewport(uint32_t width, uint32_t height);
		void set_orthographic(float size, float near_clip, float far_clip);

		void set_orthographic_size(float orthographic_size)
		{
			m_orthographic_size = orthographic_size;
			RecalculateProjection();
		}
		float get_orthographic_size() const
		{
			return m_orthographic_size;
		}

	private:
		void RecalculateProjection();

		float m_orthographic_size{ 10 };
		float m_orthographic_near{ -1 };
		float m_orthographic_far{ 1 };
		float m_aspect_ration;
	};
}
