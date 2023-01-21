#include <Engine/Renderer/Renderer2D.h>
#include "Sandbox2D.h"

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
	/*ImGui::ColorEdit4("color", &m_uniform_color.r);
	ImVec2 size = { 500, 100 };
	ImGui::SetWindowSize(size);*/
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

	Engine::Renderer2D::BeginScene(m_camera_controller.get_camera());

	Engine::Renderer2D::DrawQuad({ 0, 0, -1 }, { 10, 10 }, m_texture, { 0.5f, 0.5f, 1, 1 });
	Engine::Renderer2D::DrawQuad({ 1, 0, 1 }, { 0.7f, 1 }, { 0.8f, 0.2f, 0.3f, 1 });
	Engine::Renderer2D::DrawQuad({ -2, 0, 1 }, { 1, 1 }, { 0.3f, 0.2f, 0.75f, 1 });

	Engine::Renderer2D::EndScene();
}


