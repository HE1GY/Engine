#include <Engine/Renderer/Renderer2D.h>
#include "Sandbox2D.h"

#include <random>

Sandbox2D::Sandbox2D()
		:Layer("Sandbox2D"), m_camera_controller(1280.0f / 720.0f, true)
{
}

void Sandbox2D::OnAttach()
{
	m_texture = Engine::Texture2D::Create("../../Sandbox/assets/textures/chess.png");
	srand(time(NULL));
}

void Sandbox2D::OnDetach()
{
}

void Sandbox2D::OnImGuiRender()
{
	ImGui::SetWindowSize({ 100, 50 });
	ImGui::Text("Score: %d", m_score);

}
void Sandbox2D::OnEvent(Engine::Event& event)
{
	m_camera_controller.OnEvent(event);
}
void Sandbox2D::OnUpdate(Engine::TimeStep ts)
{
	m_camera_controller.OnUpdate(ts);

	m_player.rotation -= 40 * ts;
	m_player.rotation = std::max(m_player.rotation, -91.0f);
	{
		if (Engine::Input::IsKeyPress(KEY_SPACE) && !m_jumping)
		{
			m_velocity.y = 250 * ts;
			m_jumping = true;
			m_player.rotation = 0;
		}
		else if (!Engine::Input::IsKeyPress(KEY_SPACE) && m_jumping)
		{
			m_jumping = false;
		}
		m_velocity.y -= 5 * ts;

		m_player.position += m_velocity * ts.GetInSeconds();
	}


	///env///

	m_env_x_pos -= 1 * ts;
	if (m_env_x_pos <= -5)
	{
		m_env_x_pos = 0;
		m_score++;

		float random_numb = rand() % 3 - 1;
		TRACE(random_numb);
		m_top_pillar.position.y = 4 + random_numb;
		m_bottom_pillar.position.y = -4 + random_numb;
	}

	m_ground.position.x = m_env_x_pos;
	m_ceiling.position.x = m_env_x_pos;

	m_top_pillar.position.x = m_env_x_pos + 3;
	m_bottom_pillar.position.x = m_env_x_pos + 3;




	///collision//

	if (Collide(m_ground, m_player) || Collide(m_ceiling, m_player) || Collide(m_bottom_pillar, m_player)
			|| Collide(m_top_pillar, m_player))
	{
		m_player.position = { 0, 0, 0 };
		m_velocity = { 0, 2, 0 };
		m_player.rotation = 0;
		m_score = 0;

		m_ceiling.position = { 0, 4, 0 };
		m_ground.position = { 0, -4, 0 };
	}

	{
		Engine::RendererCommand::SetClearColor({ 0.1f, 0.1f, 0.1f, 0.1f });
		Engine::RendererCommand::Clear();

		Engine::Renderer2D::BeginScene(m_camera_controller.get_camera());

		Engine::Renderer2D::DrawQuad({ 0, 0, -1 }, { 10, 10 }, m_texture);

		{
			Engine::Renderer2D::DrawQuad(m_player.position, m_player.scale, m_player.rotation, m_player.color);

			Engine::Renderer2D::DrawQuad(m_ground.position, m_ground.scale, m_ground.rotation, m_ground.color);
			Engine::Renderer2D::DrawQuad(m_ceiling.position, m_ceiling.scale, m_ceiling.rotation, m_ceiling.color);

			Engine::Renderer2D::DrawQuad(m_top_pillar.position, m_top_pillar.scale, m_top_pillar.rotation,
					m_top_pillar.color);
			Engine::Renderer2D::DrawQuad(m_bottom_pillar.position, m_bottom_pillar.scale, m_bottom_pillar.rotation,
					m_bottom_pillar.color);
		}

		Engine::Renderer2D::EndScene();
	}
}

static bool Within(float value, float low_bound, float top_bound)
{
	return (low_bound <= value) && (top_bound >= value);
}

bool Sandbox2D::Collide(const Quad& quad1, const Quad& quad2)
{
	return (((Within(quad1.get_left_bound(), quad2.get_left_bound(), quad2.get_right_bound()) ||
			Within(quad1.get_right_bound(), quad2.get_left_bound(), quad2.get_right_bound()))
			&&
					(Within(quad1.get_bottom_bound(), quad2.get_bottom_bound(), quad2.get_top_bound()) ||
							Within(quad1.get_top_bound(), quad2.get_bottom_bound(), quad2.get_top_bound())))
			||
					(Within(quad2.get_left_bound(), quad1.get_left_bound(), quad1.get_right_bound()) ||
							Within(quad2.get_right_bound(), quad1.get_left_bound(), quad1.get_right_bound()))
							&&
									(Within(quad2.get_bottom_bound(), quad1.get_bottom_bound(), quad1.get_top_bound())
											||
													Within(quad2.get_top_bound(), quad1.get_bottom_bound(),
															quad1.get_top_bound())));
}




