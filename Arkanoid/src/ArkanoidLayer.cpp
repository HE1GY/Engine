#include "ArkanoidLayer.h"

namespace Arkanoid
{
	static glm::vec2
	CheckCollision(const glm::vec2& quad_pos, const glm::vec2& quad_size, const glm::vec2& circle_pos,
			const glm::vec2& circle_size)
	{
		float closest_x = glm::max(quad_pos.x - (quad_size.x / 2),
				glm::min(quad_pos.x + (quad_size.x / 2), circle_pos.x));
		float closest_y = glm::max(quad_pos.y - (quad_size.y / 2),
				glm::min(quad_pos.y + (quad_size.y / 2), circle_pos.y));

		float length = glm::length(circle_pos - glm::vec2{ closest_x, closest_y });
		if (length < (circle_size.x / 2))
		{
			return glm::normalize(circle_pos - glm::vec2{ closest_x, closest_y });
		}
		return glm::vec2{ 0, 0 };
	}

	ArkanoidLayer::ArkanoidLayer()
			:Layer("Arkanoid"), m_camera(-s_aspect_ration, s_aspect_ration, -1, 1)
	{
	}

	void ArkanoidLayer::OnAttach()
	{
		m_player_platform_texture = Texture2D::Create("../../../Arkanoid/assets/textures/31-Breakout-Tiles.png");
		m_ball_texture = Texture2D::Create("../../../Arkanoid/assets/textures/63-Breakout-Tiles.png");
		m_platform_texture = Texture2D::Create("../../../Arkanoid/assets/textures/09-Breakout-Tiles.png");

		m_player_pos = { 0, -0.9 };

		m_scene = CreateRef<Scene>();

		Entity camera = m_scene->CreateEntity("Main Camera");
		auto& camera_cmp = camera.AddComponent<CameraComponent>();
		camera_cmp.primary = true;
		camera_cmp.camera.SetProjectionType(SceneCamera::ProjectionType::Orthographic);
		camera_cmp.camera.SetOrthographic(1, 1, -1);
		camera_cmp.camera.SetViewport(1280, 720);
	}

	void ArkanoidLayer::OnUpdate(TimeStep ts)
	{
		/*RendererCommand::SetClearColor({ 0.1f, 0.1f, 0.1f, 1.0f });
		RendererCommand::Clear();

		const float player_speed = 1;
		if (Input::IsKeyPress(KeyCode::E_KEY_A))
		{
			m_player_pos.x -= player_speed * ts;
		}
		else if (Input::IsKeyPress(KeyCode::E_KEY_D))
		{
			m_player_pos.x += player_speed * ts;
		}
		m_player_pos.x = std::clamp(m_player_pos.x, -s_aspect_ration + (m_player_size.x / 2.0f),
				s_aspect_ration - (m_player_size.x / 2.0f));

		const float ball_speed = 1.5f;
		m_ball_pos += m_ball_velocity * 0.005f * ball_speed;

		static bool wall_bounce_side{ false };
		if ((m_ball_pos.x > s_aspect_ration - (m_ball_size.x / 2.0f) && !wall_bounce_side)
				|| (m_ball_pos.x < -s_aspect_ration + (m_ball_size.x / 2.0f) && !wall_bounce_side))
		{
			wall_bounce_side = true;
			m_ball_velocity.x *= -1;
		}
		else
		{
			wall_bounce_side = false;
		}

		static bool wall_bounce_floor{ false };
		if ((m_ball_pos.y > 1 - (m_ball_size.y / 2.0f) && !wall_bounce_floor)
				|| (m_ball_pos.y < -1 + (m_ball_size.y / 2.0f) && !wall_bounce_floor))
		{
			wall_bounce_floor = true;
			m_ball_velocity.y *= -1;
		}
		else
		{
			wall_bounce_floor = false;
		}

		static bool collision{ false };
		glm::vec2 normal = CheckCollision(m_player_pos, m_player_size, m_ball_pos, m_ball_size);

		if (normal != glm::vec2{ 0, 0 } && !collision)
		{
			collision = true;
			m_ball_velocity = glm::reflect(m_ball_velocity, normal);
		}
		else
		{
			collision = false;
		}

		Renderer2D::BeginScene(m_camera);

		Renderer2D::DrawQuad(m_player_pos, m_player_size, m_player_platform_texture);
		Renderer2D::DrawQuad(m_ball_pos, m_ball_size, m_ball_texture);

		glm::vec2 start_pos = { -s_aspect_ration + m_platform_size.x + 0.1, 1 - 0.1 };

		static bool active[10]{ true, true, true, true, true, true, true, true, true, true };

		for (int i = 0; i < 10; ++i)
		{
			if (active[i])
			{
				glm::vec2 pos = { start_pos.x + i * (m_platform_size.x + 0.1f), start_pos.y };
				glm::vec2 normal_2 = CheckCollision(pos, m_player_size, m_ball_pos, m_ball_size);
				if (normal_2 != glm::vec2{ 0, 0 } && !collision)
				{
					collision = true;
					m_ball_velocity = glm::reflect(m_ball_velocity, normal_2);
					active[i] = false;
				}
				else
				{
					collision = false;
				}

				Renderer2D::DrawQuad(pos, m_platform_size, m_platform_texture);
			}
		}

		Renderer2D::EndScene();*/

		m_scene->OnUpdateRuntime(ts);
	}

	void ArkanoidLayer::OnEvent(Event& event)
	{
		EventDispatcher ed(event);
		ed.Dispatch<WindowResized>(BIND_EVENT_FUNC(ArkanoidLayer::OnResize));
	}

	void ArkanoidLayer::OnImGuiRender()
	{

	}

	bool ArkanoidLayer::OnResize(WindowResized& event)
	{
		m_scene->OnViewResize(event.GetWidth(), event.GetHeight());
		return false;
	}

}
