#include <Engine/Renderer/Renderer2D.h>
#include "Sandbox2D.h"

Sandbox2D::Sandbox2D()
		:Layer("Sandbox2D"), m_camera_controller(1280.0f / 720.0f, true)
{
}

void Sandbox2D::OnAttach()
{
	m_texture = Engine::Texture2D::Create("../../Sandbox/assets/textures/chess.png");

}

void Sandbox2D::OnDetach()
{
}

void Sandbox2D::OnImGuiRender()
{

}
void Sandbox2D::OnEvent(Engine::Event& event)
{
	m_camera_controller.OnEvent(event);
}
void Sandbox2D::OnUpdate(Engine::TimeStep ts)
{
	m_camera_controller.OnUpdate(ts);

	m_player.rotation -= 75 * ts;
	m_player.rotation = std::max(m_player.rotation, -91.0f);
	{
		if (Engine::Input::IsKeyPress(KEY_SPACE) && !m_jumping)
		{
			m_velocity.y = 400 * ts;
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

	m_env_pos -= 1 * ts;
	if (m_env_pos <= -5)
	{
		m_env_pos = 0;
	}

	m_ground.position.x = m_env_pos;
	m_ceiling.position.x = m_env_pos;


	///collision//

	{
		Engine::RendererCommand::SetClearColor({ 0.1f, 0.1f, 0.1f, 0.1f });
		Engine::RendererCommand::Clear();

		Engine::Renderer2D::BeginScene(m_camera_controller.get_camera());

		Engine::Renderer2D::DrawQuad({ 0, 0, -1 }, { 10, 10 }, m_texture);

		{
			Engine::Renderer2D::DrawQuad(m_player.position, m_player.scale, m_player.rotation, m_player.color);

			Engine::Renderer2D::DrawQuad(m_ground.position, m_ground.scale, m_ground.rotation, m_ground.color);
			Engine::Renderer2D::DrawQuad(m_ceiling.position, m_ceiling.scale, m_ceiling.rotation, m_ceiling.color);
		}

		Engine::Renderer2D::EndScene();
	}
}


