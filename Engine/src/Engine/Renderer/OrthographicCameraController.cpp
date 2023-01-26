#include "OrthographicCameraController.h"
#include "Engine/Core/Input.h"
#include "Engine/Core/KeyCode.h"

namespace Engine
{
	OrthographicCameraController::OrthographicCameraController(float aspect_ratio, bool is_rotating)
			:m_aspect_ratio{ aspect_ratio }, m_is_rotating{ is_rotating },
			 m_bound{ -m_aspect_ratio * m_zoom_level, m_aspect_ratio * m_zoom_level, -m_zoom_level, m_zoom_level },
			 m_camera(m_bound.left, m_bound.right, m_bound.bottom, m_bound.top)
	{
	}
	void OrthographicCameraController::OnUpdate(TimeStep ts)
	{
		PROFILER_FUNCTION();

		glm::vec3 cam_pos = m_camera.get_position();
		float cam_rot = m_camera.get_rotation();
		float speed = 5.0f * m_zoom_level;
		float speed_rot = 30.0f;

		if (Engine::Input::IsKeyPress(KEY_A))
		{
			cam_pos.x -= speed * ts;
		}
		if (Engine::Input::IsKeyPress(KEY_D))
		{
			cam_pos.x += speed * ts;
		}
		if (Engine::Input::IsKeyPress(KEY_W))
		{
			cam_pos.y += speed * ts;
		}
		if (Engine::Input::IsKeyPress(KEY_S))
		{
			cam_pos.y -= speed * ts;
		}
		if (m_is_rotating)
		{
			if (Engine::Input::IsKeyPress(KEY_Q))
			{
				cam_rot += speed_rot * ts;
			}
			if (Engine::Input::IsKeyPress(KEY_E))
			{
				cam_rot -= speed_rot * ts;
			}
			m_camera.set_rotation(cam_rot);
		}

		m_camera.set_position(cam_pos);
	}
	void OrthographicCameraController::OnEvent(Event& event)
	{
		EventDispatcher ed(event);
		ed.Dispatch<MouseScrolled>(BIND_EVENT_FUNC(OrthographicCameraController::OnMouseScroll));
		ed.Dispatch<WindowResized>(BIND_EVENT_FUNC(OrthographicCameraController::OnWindowResized));
	}
	bool OrthographicCameraController::OnMouseScroll(MouseScrolled& event)
	{
		m_zoom_level -= event.get_y_offset() * 0.2f;
		m_zoom_level = std::max(m_zoom_level, 0.25f);
		m_bound = { -m_aspect_ratio * m_zoom_level, m_aspect_ratio * m_zoom_level, -m_zoom_level,
					m_zoom_level };
		m_camera.set_projection(m_bound.left, m_bound.right, m_bound.bottom, m_bound.top);
		return false;
	}
	bool OrthographicCameraController::OnWindowResized(WindowResized& event)
	{
		OnResize(event.get_width(), event.get_height());
		return false;
	}
	void OrthographicCameraController::OnResize(float width, float height)
	{
		m_aspect_ratio = width / height;
		m_bound = { -m_aspect_ratio * m_zoom_level, m_aspect_ratio * m_zoom_level, -m_zoom_level,
					m_zoom_level };
		m_camera.set_projection(m_bound.left, m_bound.right, m_bound.bottom, m_bound.top);
	}
} // namespace Engine
