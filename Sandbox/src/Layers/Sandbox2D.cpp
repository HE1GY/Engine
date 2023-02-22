#include "Engine/Renderer/Renderer2D.h"
#include "Sandbox2D.h"

#include <random>

Sandbox2D::Sandbox2D()
		:Layer("Sandbox2D"), m_camera_controller(1280.0f / 720.0f, true)
{
}

void Sandbox2D::OnAttach()
{
	m_texture_chess = Engine::Texture2D::Create("../../../Sandbox/assets/textures/chess.png");
	m_texture_sprites = Engine::Texture2D::Create("../../../Sandbox/assets/textures/RPGpack_sheet_2X.png");
	m_wall = Engine::SubTexture2D::CreateFromCoord(m_texture_sprites, { 9, 9 }, { 128, 128 }, { 128, 128 });
	m_tree = Engine::SubTexture2D::CreateFromCoord(m_texture_sprites, { 0, 1 }, { 128, 256 }, { 128, 128 });

	ParticlesProp prop;
	prop.life_time = 2;
	prop.start_size = { 0.05, 0.05, 1 };
	prop.end_size = { 0.005, 0.005, 1 };
	prop.start_color = glm::vec4{ 0.9, 0.3, 0.1, 1 };
	prop.end_color = glm::vec4{ 0.7, 0.2, 0.1, 0.1 };
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

		float width = Engine::Application::get()->GetWindow()->get_width();
		float height = Engine::Application::get()->GetWindow()->get_height();

		glm::vec4 clip_space = { (x / width) * 2 - 1, 1 - 2 * (y / height), 0, 1 }; // to get -1<x<1 and -1<y<1

		glm::mat4 inverse_view_proj = glm::inverse(m_camera_controller.get_camera().GetViewProjectionMatrix());
		glm::vec4 world_space = inverse_view_proj * clip_space;
		world_space /= world_space.w;

		for (int i = 0; i < 50; ++i)
		{
			m_particles.Emit({ world_space.x, world_space.y, 0 });
		}
	}

	m_fps = (float)1 / ts;
	m_camera_controller.OnUpdate(ts);

	Engine::RendererCommand::SetClearColor({ 0.1f, 0.1f, 0.1f, 0.1f });
	Engine::RendererCommand::Clear();

	Engine::Renderer2D::ResetStats();

	{

		Engine::Renderer2D::BeginScene(m_camera_controller.get_camera());

		{
			static float color_anim{ 0.1f };
			static bool increase{ true };
			static float color_step{ 0.3f };

			color_anim += ts.get_In_seconds() * color_step;

			if (color_anim >= 1 && increase)
			{
				increase = false;
				color_step *= -1;
			}
			if (color_anim <= 0 && !increase)
			{
				increase = true;
				color_step *= -1;
			}

			for (int i = 0; i < 50; ++i)
			{
				for (int j = 0; j < 50; ++j)
				{
					float r = i / float(50) * color_anim;
					float g = j / float(50) * color_anim;
					float b = j / float(50) * color_anim;
					Engine::Renderer2D::DrawQuad({ i + 5, j, 0 }, { 0.7, 0.7 }, { r, g, b, 1 });
				}
			}
		}

		Engine::Renderer2D::DrawQuad({ 0, 0, 0 }, { 1, 1 }, m_wall);
		Engine::Renderer2D::DrawQuad({ -1, 0.5f, 0 }, { 1, 2 }, m_tree);

		Engine::Renderer2D::EndScene();

		m_particles.OnUpdate(ts);
		m_particles.OnRenderer(m_camera_controller.get_camera());
	}
}






