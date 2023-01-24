#pragma once

#include "Engine/Core/TimeStep.h"
#include "Engine/Event/ApplicationEvent.h"
#include "Engine/Event/Event.h"
#include "Engine/Event/MouseEvent.h"

#include "Engine/Renderer/OrthographicCamera.h"

namespace Engine
{
	class OrthographicCameraController
	{
		struct OrthographicCameraBound
		{
			float left, right;
			float bottom, top;
			float get_width() const
			{
				return right - left;
			}
			float get_height() const
			{
				return top - bottom;
			}
		};
	public:
		OrthographicCameraController(float aspect_ratio, bool is_rotating);

		void OnUpdate(TimeStep ts);
		void OnEvent(Event& event);

		const OrthographicCamera& get_camera() const
		{
			return m_camera;
		}

		OrthographicCamera& get_camera()
		{
			return m_camera;
		}
		const OrthographicCameraBound& get_bound() const
		{
			return m_bound;
		}

	private:
		bool OnMouseScroll(MouseScrolled& event);
		bool OnWindowResized(WindowResized& event);

	private:
		float m_aspect_ratio;
		bool m_is_rotating;
		float m_zoom_level{ 1 };

		OrthographicCameraBound m_bound;
		OrthographicCamera m_camera;
	};
} // namespace Engine