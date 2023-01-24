#include "Engine/Renderer/Renderer2D.h"
#include "Sandbox2D.h"

#include <random>

Sandbox2D::Sandbox2D()
		:Layer("Sandbox2D"), m_camera_controller(1280.0f / 720.0f, true)
{
}

void Sandbox2D::OnAttach()
{
	m_texture = Engine::Texture2D::Create("../../../Sandbox/assets/textures/chess.png");

	ParticlesProp prop;
	prop.life_time = 2;
	prop.size = { 0.05, 0.05, 1 };
	prop.start_color = glm::vec4{ 1, 0, 0, 1 };
	prop.end_color = glm::vec4{ 0, 1, 0, 0.1 };
	m_particles.Init(prop);
}

void Sandbox2D::OnDetach()
{
}

void Sandbox2D::OnImGuiRender()
{
	auto stats = Engine::Renderer2D::GetStats();

	ImGui::Begin("Settings");

	ImGui::Text("FPS: %d", (int)m_fps);
	ImGui::Text("draw calls: %d", stats.draw_calls);
	ImGui::Text("quads: %d", stats.quads);
	ImGui::Text("indices: %d", stats.get_indices());
	ImGui::Text("vertices: %d", stats.get_vertices());

	ImGui::End();
}
void Sandbox2D::OnEvent(Engine::Event& event)
{
	m_camera_controller.OnEvent(event);
}
void Sandbox2D::OnUpdate(Engine::TimeStep ts)
{
	if (Engine::Input::IsMouseButtonPress(MOUSE_BUTTON_LEFT))
	{
		auto [x, y] = Engine::Input::GetMousePos();

		float width = Engine::Application::Get()->GetWindow()->get_width();
		float height = Engine::Application::Get()->GetWindow()->get_height();

		float bound_width = m_camera_controller.get_bound().get_width();
		float bound_height = m_camera_controller.get_bound().get_height();

		auto& cam_pos = m_camera_controller.get_camera().get_position();

		x = (x / width) * bound_width - (bound_width * 0.5f);
		y = (bound_height * 0.5f) - (y / height) * bound_height;

		x += cam_pos.x;
		y += cam_pos.y;

		for (int i = 0; i < 50; ++i)
		{
			m_particles.Emit({ x, y, 0 });
		}

	}

	m_fps = (float)1 / ts;
	m_camera_controller.OnUpdate(ts);

	Engine::RendererCommand::SetClearColor({ 0.1f, 0.1f, 0.1f, 0.1f });
	Engine::RendererCommand::Clear();

	Engine::Renderer2D::ResetStats();

	{
		static float angle = 0.0f;
		angle += 100 * ts;

		Engine::Renderer2D::BeginScene(m_camera_controller.get_camera());

		Engine::Renderer2D::DrawQuad({ 0, 0, -1 }, { 5, 5 }, m_texture);

		Engine::Renderer2D::DrawQuad({ 1, 0, 0.1 }, { 1, 1 }, { 0.8, 0.2, 0.3, 1 });

		Engine::Renderer2D::DrawQuad({ -1, 0, 0 }, { 1, 1 }, { 0.2, 0.2, 0.3, 1 });

		Engine::Renderer2D::DrawQuad({ 0, 0, 0 }, { 1, 1 }, angle, { 0.5, 0.5, 0.5, 1 });

		static float color_anim = 0.1f;

		color_anim += ts * 0.5f;

		if (color_anim >= 1)color_anim = 0.1f;

		for (int i = 0; i < 100; ++i)
		{
			for (int j = 0; j < 100; ++j)
			{
				float r = i / float(100) * color_anim;
				float g = j / float(100) * color_anim;
				float b = j / float(100) * color_anim;
				Engine::Renderer2D::DrawQuad({ i + 5, j, 0 }, { 0.7, 0.7 }, { r, g, b, 1 });
			}
		}
		Engine::Renderer2D::EndScene();

		m_particles.OnUpdate(ts);
		m_particles.OnRenderer(m_camera_controller.get_camera());
	}
}






