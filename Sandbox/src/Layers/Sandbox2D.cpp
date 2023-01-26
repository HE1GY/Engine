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

	Engine::FrameBufferSpecification fb_spec;
	fb_spec.width = 1280;
	fb_spec.height = 720;

	m_frame_buffer = Engine::FrameBuffer::Create(fb_spec);

}

void Sandbox2D::OnDetach()
{
}

void Sandbox2D::OnImGuiRender()
{
	// If you strip some features of, this demo is pretty much equivalent to calling DockSpaceOverViewport()!
	// In most cases you should be able to just call DockSpaceOverViewport() and ignore all the code below!
	// In this specific demo, we are not using DockSpaceOverViewport() because:
	// - we allow the host window to be floating/moveable instead of filling the viewport (when opt_fullscreen == false)
	// - we allow the host window to have padding (when opt_padding == true)
	// - we have a local menu bar in the host window (vs. you could use BeginMainMenuBar() + DockSpaceOverViewport() in your code!)
	// TL;DR; this demo is more complicated than what you would normally use.
	// If we removed all the options we are showcasing, this demo would become:
	//     void ShowExampleAppDockSpace()
	//     {
	//         ImGui::DockSpaceOverViewport(ImGui::GetMainViewport());
	//     }

	static bool opt_fullscreen = true;
	static bool p_open_val = true;
	static bool* p_open = &p_open_val;
	static bool opt_padding = false;
	static ImGuiDockNodeFlags dockspace_flags = ImGuiDockNodeFlags_None;

	// We are using the ImGuiWindowFlags_NoDocking flag to make the parent window not dockable into,
	// because it would be confusing to have two docking targets within each others.
	ImGuiWindowFlags window_flags = ImGuiWindowFlags_MenuBar | ImGuiWindowFlags_NoDocking;
	if (opt_fullscreen)
	{
		const ImGuiViewport* viewport = ImGui::GetMainViewport();
		ImGui::SetNextWindowPos(viewport->WorkPos);
		ImGui::SetNextWindowSize(viewport->WorkSize);
		ImGui::SetNextWindowViewport(viewport->ID);
		ImGui::PushStyleVar(ImGuiStyleVar_WindowRounding, 0.0f);
		ImGui::PushStyleVar(ImGuiStyleVar_WindowBorderSize, 0.0f);
		window_flags |= ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoResize
				| ImGuiWindowFlags_NoMove;
		window_flags |= ImGuiWindowFlags_NoBringToFrontOnFocus | ImGuiWindowFlags_NoNavFocus;
	}
	else
	{
		dockspace_flags &= ~ImGuiDockNodeFlags_PassthruCentralNode;
	}

	// When using ImGuiDockNodeFlags_PassthruCentralNode, DockSpace() will render our background
	// and handle the pass-thru hole, so we ask Begin() to not render a background.
	if (dockspace_flags & ImGuiDockNodeFlags_PassthruCentralNode)
		window_flags |= ImGuiWindowFlags_NoBackground;

	// Important: note that we proceed even if Begin() returns false (aka window is collapsed).
	// This is because we want to keep our DockSpace() active. If a DockSpace() is inactive,
	// all active windows docked into it will lose their parent and become undocked.
	// We cannot preserve the docking relationship between an active window and an inactive docking, otherwise
	// any change of dockspace/settings would lead to windows being stuck in limbo and never being visible.
	if (!opt_padding)
		ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(0.0f, 0.0f));
	ImGui::Begin("DockSpace Demo", p_open, window_flags);
	if (!opt_padding)
		ImGui::PopStyleVar();

	if (opt_fullscreen)
		ImGui::PopStyleVar(2);

	// Submit the DockSpace
	ImGuiIO& io = ImGui::GetIO();
	if (io.ConfigFlags & ImGuiConfigFlags_DockingEnable)
	{
		ImGuiID dockspace_id = ImGui::GetID("MyDockSpace");
		ImGui::DockSpace(dockspace_id, ImVec2(0.0f, 0.0f), dockspace_flags);
	}

	if (ImGui::BeginMenuBar())
	{
		if (ImGui::BeginMenu("Options"))
		{
			// Disabling fullscreen would allow the window to be moved to the front of other windows,
			// which we can't undo at the moment without finer window depth/z control.
			ImGui::MenuItem("Fullscreen", NULL, &opt_fullscreen);
			ImGui::MenuItem("Padding", NULL, &opt_padding);
			ImGui::Separator();

			if (ImGui::MenuItem("Flag: NoSplit", "", (dockspace_flags & ImGuiDockNodeFlags_NoSplit) != 0))
			{ dockspace_flags ^= ImGuiDockNodeFlags_NoSplit; }
			if (ImGui::MenuItem("Flag: NoResize", "", (dockspace_flags & ImGuiDockNodeFlags_NoResize) != 0))
			{ dockspace_flags ^= ImGuiDockNodeFlags_NoResize; }
			if (ImGui::MenuItem("Flag: NoDockingInCentralNode", "",
					(dockspace_flags & ImGuiDockNodeFlags_NoDockingInCentralNode) != 0))
			{ dockspace_flags ^= ImGuiDockNodeFlags_NoDockingInCentralNode; }
			if (ImGui::MenuItem("Flag: AutoHideTabBar", "", (dockspace_flags & ImGuiDockNodeFlags_AutoHideTabBar) != 0))
			{ dockspace_flags ^= ImGuiDockNodeFlags_AutoHideTabBar; }
			if (ImGui::MenuItem("Flag: PassthruCentralNode", "",
					(dockspace_flags & ImGuiDockNodeFlags_PassthruCentralNode) != 0, opt_fullscreen))
			{ dockspace_flags ^= ImGuiDockNodeFlags_PassthruCentralNode; }
			ImGui::Separator();

			if (ImGui::MenuItem("Close", NULL, false, p_open != NULL))
				*p_open = false;
			ImGui::EndMenu();
		}

		ImGui::EndMenuBar();
	}

	auto stats = Engine::Renderer2D::GetStats();

	ImGui::Begin("Viewport");

	uint32_t tex_id = m_frame_buffer->get_color_attachment_renderer_id();
	ImGui::Image((void*)tex_id, ImVec2{ 1280, 720 }, ImVec2{ 0, 1 }, ImVec2{ 1, 0 });

	ImGui::End();

	ImGui::Begin("Settings");

	ImGui::Text("FPS: %d", (int)m_fps);
	ImGui::Text("draw calls: %d", stats.draw_calls);
	ImGui::Text("quads: %d", stats.quads);
	ImGui::Text("indices: %d", stats.get_indices());
	ImGui::Text("vertices: %d", stats.get_vertices());

	ImGui::End();
 
	ImGui::End();

}
void Sandbox2D::OnEvent(Engine::Event& event)
{
	m_camera_controller.OnEvent(event);
}
void Sandbox2D::OnUpdate(Engine::TimeStep ts)
{
	m_frame_buffer->Bind();

	if (Engine::Input::IsMouseButtonPress(MOUSE_BUTTON_LEFT))
	{
		auto [x, y] = Engine::Input::GetMousePos();

		float width = Engine::Application::Get()->GetWindow()->get_width();
		float height = Engine::Application::Get()->GetWindow()->get_height();

		glm::vec4 clip_space = { (x / width) * 2 - 1, 1 - 2 * (y / height), 0, 1 }; // to get -1<x<1 and -1<y<1

		glm::mat4 inverse_view_proj = glm::inverse(m_camera_controller.get_camera().get_view_projection_matrix());
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

		m_frame_buffer->UnBind();
	}
}






