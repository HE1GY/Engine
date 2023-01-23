#include <Engine/Renderer/Renderer2D.h>
#include "Sandbox2D.h"

#include <random>

Sandbox2D::Sandbox2D()
		:Layer("Sandbox2D"), m_camera_controller(1280.0f / 720.0f, true)
{
}

void Sandbox2D::OnAttach()
{
	m_texture = Engine::Texture2D::Create("../../../Sandbox/assets/textures/chess.png");
}

void Sandbox2D::OnDetach()
{
}

void Sandbox2D::OnImGuiRender()
{
	auto stats = Engine::Renderer2D::GetStats();

	ImGui::Begin("Settings");

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
	m_camera_controller.OnUpdate(ts);

	Engine::RendererCommand::SetClearColor({ 0.1f, 0.1f, 0.1f, 0.1f });
	Engine::RendererCommand::Clear();

	Engine::Renderer2D::ResetStats();

	{
		static float angle = 0.0f;

		angle += 100 * ts;

		Engine::Renderer2D::BeginScene(m_camera_controller.get_camera());

		Engine::Renderer2D::DrawQuad({ 0, 0, -1 }, { 5, 5 }, { 0.2, 0.2, 0.2, 1 });

		Engine::Renderer2D::DrawQuad({ 1, 0, 0.1 }, { 1, 1 }, { 0.8, 0.2, 0.3, 1 });

		Engine::Renderer2D::DrawQuad({ -1, 0, 0 }, { 1, 1 }, { 0.2, 0.2, 0.3, 1 });

		Engine::Renderer2D::DrawQuad({ 0, 0, 0 }, { 1, 1 }, angle, { 0.5, 0.5, 0.5, 1 });

		for (int i = 0; i < 50; ++i)
		{
			for (int j = 0; j < 50; ++j)
			{
				float r = i / (float)50;
				float g = j / float(50);
				Engine::Renderer2D::DrawQuad({ i, j, 0 }, { 0.7, 0.7 }, { r, g, 0.3, 1 });
			}
		}

		Engine::Renderer2D::EndScene();
	}
}






