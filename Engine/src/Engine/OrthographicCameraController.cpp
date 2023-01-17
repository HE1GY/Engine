#include "OrthographicCameraController.h"
#include "Input.h"
#include "KeyCode.h"

namespace Engine
{

	Engine::OrthographicCameraController::OrthographicCameraController(float aspect_ratio, bool is_rotating)
			:m_aspect_ratio{ aspect_ratio }, m_is_rotating{ is_rotating },
			 m_camera(-m_aspect_ratio * m_zoom_level, m_aspect_ratio * m_zoom_level, -m_zoom_level, m_zoom_level)
	{
	}
	void OrthographicCameraController::OnUpdate(TimeStep ts)
	{
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
		m_camera.set_projection(-m_aspect_ratio * m_zoom_level, m_aspect_ratio * m_zoom_level, -m_zoom_level,
				m_zoom_level);
		return false;
	}
	bool OrthographicCameraController::OnWindowResized(WindowResized& event)
	{
		m_aspect_ratio = (float)event.get_width() / (float)event.get_height();
		m_camera.set_projection(-m_aspect_ratio * m_zoom_level, m_aspect_ratio * m_zoom_level, -m_zoom_level,
				m_zoom_level);
		return false;
	}
} // namespace Engine