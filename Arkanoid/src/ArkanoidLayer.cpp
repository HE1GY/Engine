#include "ArkanoidLayer.h"

namespace Arkanoid
{
	static void
	ResolveQuadCircleCollision(const glm::vec2& quad_pos, const glm::vec2& quad_size, const glm::vec2& circle_pos,
			const glm::vec2& circle_size, glm::vec2& circle_velocity)
	{
		float distance_x = abs(circle_pos.x - quad_pos.x);
		float distance_y = abs(circle_pos.y - quad_pos.y);

		if (distance_x < (circle_size.x / 2.0f) + (quad_size.x / 2.0f)
				&& distance_y < (circle_size.y / 2.0f) + (quad_size.y / 2.0f))
		{
			glm::vec2 compass[4];
			compass[0] = { 1, 0 };
			compass[1] = { 0, 1 };
			compass[2] = { -1, 0 };
			compass[3] = { 0, -1 };

			float dot_prod = 1;
			uint32_t index = 0;
			for (int i = 0; i < 4; ++i)
			{
				float result = glm::dot(compass[i], glm::normalize(circle_velocity));
				if (result < dot_prod)
				{
					dot_prod = result;
					index = i;
				}
			}

			circle_velocity = glm::normalize(glm::reflect(circle_velocity, compass[index]));
		}

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
	}

	void ArkanoidLayer::OnUpdate(TimeStep ts)
	{
		RendererCommand::SetClearColor({ 0.1f, 0.1f, 0.1f, 1.0f });
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
		m_ball_pos += m_ball_velocity * ts.GetInSeconds() * ball_speed;

		if (m_ball_pos.x > s_aspect_ration - (m_ball_size.x / 2.0f)
				|| m_ball_pos.x < -s_aspect_ration + (m_ball_size.x / 2.0f))
		{
			m_ball_velocity.x *= -1;
		}
		if (m_ball_pos.y > 1 - (m_ball_size.y / 2.0f)
				|| m_ball_pos.y < -1 + (m_ball_size.y / 2.0f))
		{
			m_ball_velocity.y *= -1;
		}

		ResolveQuadCircleCollision(m_player_pos, m_player_size, m_ball_pos, m_ball_size, m_ball_velocity);

		Renderer2D::BeginScene(m_camera);

		Renderer2D::DrawQuad(m_player_pos, m_player_size, m_player_platform_texture);
		Renderer2D::DrawQuad(m_ball_pos, m_ball_size, m_ball_texture);

		glm::vec2 start_pos = { -s_aspect_ration + m_platform_size.x + 0.1, 1 - 0.1 };
		Renderer2D::DrawQuad(start_pos, m_platform_size, m_platform_texture);

		for (int i = 0; i < 10; ++i)
		{
			glm::vec2 pos = { start_pos.x + i * (m_platform_size.x + 0.1f), start_pos.y };
			ResolveQuadCircleCollision(pos, m_platform_size, m_ball_pos, m_ball_size, m_ball_velocity);
			Renderer2D::DrawQuad(pos, m_platform_size, m_platform_texture);
		}

		Renderer2D::EndScene();
	}

	void ArkanoidLayer::OnEvent(Event& event)
	{

	}

	void ArkanoidLayer::OnImGuiRender()
	{

	}

}
