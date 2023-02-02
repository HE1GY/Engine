#include "pch.h"

#include <glm/ext/matrix_clip_space.hpp>
#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>

#include "Engine/Utils/FileDialogs.h"
#include "EngineEditorLayer.h"

#include "Engine/Scene/SceneSerializer.h"

#include <ImGuizmo.h>

#include "Engine/Math/Math.h"

namespace Engine
{
	EngineEditorLayer::EngineEditorLayer()
			:Layer("Engine-EditorLayer"), /*m_camera_controller(1280.0f / 720.0f, true),*/ m_scene{ CreateRef<Scene>() }
	{
	}

	void EngineEditorLayer::OnAttach()
	{
		/*m_texture_chess = Engine::Texture2D::Create("../../../Sandbox/assets/textures/chess.png");
		m_texture_sprites = Engine::Texture2D::Create("../../../Sandbox/assets/textures/RPGpack_sheet_2X.png");
		m_wall = Engine::SubTexture2D::CreateFromCoord(m_texture_sprites, { 9, 9 }, { 128, 128 }, { 128, 128 });
		m_tree = Engine::SubTexture2D::CreateFromCoord(m_texture_sprites, { 0, 1 }, { 128, 256 }, { 128, 128 });

		ParticlesProp prop;
		prop.life_time = 2;
		prop.start_size = { 0.05, 0.05, 1 };
		prop.end_size = { 0.005, 0.005, 1 };
		prop.start_color = glm::vec4{ 0.9, 0.3, 0.1, 1 };
		prop.end_color = glm::vec4{ 0.2, 0.3, 0.8, 0.1 };
		m_particles.Init(prop);*/

		FrameBufferSpecification fb_spec;
		fb_spec.width = 1280;
		fb_spec.height = 720;
		m_frame_buffer = FrameBuffer::Create(fb_spec);

		m_editor_camera = EditorCamera(30.0f, 1.778f, 0.1f, 1000.0f);

		/*Entity quad = m_scene->CreateEntity("Quad Entity");
		quad.AddComponent<SpriteRendererComponent>(glm::vec4{ 0, 1, 0, 1 });*/

		/*Entity quad2 = m_scene->CreateEntity("Quad2 Entity");
		quad2.AddComponent<SpriteRendererComponent>(glm::vec4{ 0, 0, 1, 1 });
		quad2.GetComponent<TransformComponent>().translation = { 1, 0, 0 };

		m_main_cam = m_scene->CreateEntity("main cam");
		auto& cc = m_main_cam.AddComponent<CameraComponent>();
		cc.primary = true;

		auto cam = m_scene->CreateEntity("cam2");
		cam.AddComponent<CameraComponent>();

		class CameraController : public ScriptableEntity
		{
		public:
			virtual void OnUpdate(TimeStep ts) override
			{
				auto& transform = GetComponent<TransformComponent>();

				float speed = 5.0f;

				if (Engine::Input::IsKeyPress(KEY_A))
				{
					transform.translation.x -= speed * ts;
				}
				if (Engine::Input::IsKeyPress(KEY_D))
				{
					transform.translation.x += speed * ts;
				}
				if (Engine::Input::IsKeyPress(KEY_W))
				{
					transform.translation.y += speed * ts;
				}
				if (Engine::Input::IsKeyPress(KEY_S))
				{
					transform.translation.y -= speed * ts;
				}
			}
		};

		m_main_cam.AddComponent<NativeScriptComponent>().Bind<CameraController>();*/

		m_scene_hierarchy_panel.set_context(m_scene);


		/*serializer.Deserialize("../../../EngineEditor/assets/scenes/example.engine");*/
	}
	void EngineEditorLayer::OnDetach()
	{
	}
	void EngineEditorLayer::OnUpdate(Engine::TimeStep ts)
	{

		/*if (Engine::Input::IsMouseButtonPress(MOUSE_BUTTON_LEFT))
		{
			auto [x, y] = Engine::Input::GetMousePos();

			float width = Application::get()->GetWindow()->get_width();
			float height = Application::get()->GetWindow()->get_width();

			glm::vec4 clip_space = { (x / width) * 2 - 1, 1 - 2 * (y / height), 0, 1 }; // to get -1<x<1 and -1<y<1

			glm::mat4 inverse_view_proj = glm::inverse(m_camera_controller.get_camera().get_view_projection_matrix());
			glm::vec4 world_space = inverse_view_proj * clip_space;
			world_space /= world_space.w;

			for (int i = 0; i < 50; ++i)
			{
				m_particles.Emit({ world_space.x, world_space.y, 0 });
			}
		}*/

		m_fps = (float)1 / ts;
		//m_camera_controller.OnUpdate(ts);

		{
			m_frame_buffer->Bind();

			Engine::RendererCommand::SetClearColor({ 0.1f, 0.1f, 0.1f, 0.1f });
			Engine::RendererCommand::Clear();

			Engine::Renderer2D::ResetStats();
		}
		{

			/*Engine::Renderer2D::BeginScene(m_camera_controller.get_camera());*/


			m_editor_camera.OnUpdate(ts);
			m_scene->OnUpdateEditor(ts, m_editor_camera);

			/*{
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

				for (int i = 0; i < 15; ++i)
				{
					for (int j = 0; j < 15; ++j)
					{
						float r = i / float(15) * color_anim;
						float g = j / float(15) * color_anim;
						float b = j / float(15) * color_anim;
						Engine::Renderer2D::DrawQuad({ i - 7, j - 7, 0 }, { 0.7, 0.7 }, { r, g, b, 1 });
					}
				}
			}

			Engine::Renderer2D::DrawQuad({ 0, 0, 0 }, { 1, 1 }, m_wall);
			Engine::Renderer2D::DrawQuad({ -1, 0.5f, 0 }, { 1, 2 }, m_tree);
*/
			/*Engine::Renderer2D::EndScene();*/

			/*m_particles.OnUpdate(ts);
			m_particles.OnRenderer(m_camera_controller.get_camera());*/

			m_frame_buffer->UnBind();
		}
	}
	void EngineEditorLayer::OnEvent(Event& event)
	{
		/*m_camera_controller.OnEvent(event);*/

		EventDispatcher ed(event);
		ed.Dispatch<KeyPress>(BIND_EVENT_FUNC(EngineEditorLayer::OnKeyPress));

		m_editor_camera.OnEvent(event);

	}
	void EngineEditorLayer::OnImGuiRender()
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
		ImGuiStyle& style = ImGui::GetStyle();

		float prev_window_min_size = style.WindowMinSize.x;
		style.WindowMinSize.x = 370.0f;

		if (io.ConfigFlags & ImGuiConfigFlags_DockingEnable)
		{
			ImGuiID dockspace_id = ImGui::GetID("MyDockSpace");
			ImGui::DockSpace(dockspace_id, ImVec2(0.0f, 0.0f), dockspace_flags);
		}
		style.WindowMinSize.x = prev_window_min_size;


		///////menu
		if (ImGui::BeginMenuBar())
		{
			if (ImGui::BeginMenu("File"))
			{

				if (ImGui::MenuItem("New", "Ctrl+N"))
				{
					NewScene();
				}
				if (ImGui::MenuItem("Open...", "Ctrl+O"))
				{
					OpenScene();
				}

				if (ImGui::MenuItem("Save As...", "Ctrl+Shift+S"))
				{
					SaveSceneAs();
				}

				ImGui::EndMenu();
			}

			ImGui::EndMenuBar();

		}



		/////////////////////////viewport
		ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(0, 0));

		ImGui::Begin("Viewport");

		ImVec2 size = ImGui::GetContentRegionAvail();

		if (size.x != m_viewport_size.x || size.y != m_viewport_size.y)
		{
			m_viewport_size = { size.x, size.y };
			m_frame_buffer->Resize(size.x, size.y);
			m_scene->OnViewResize(size.x, size.y);
			m_editor_camera.SetViewportSize(size.x, size.y);
			//m_camera_controller.OnViewResize(size.x, size.y);
		}
		uint32_t tex_id = m_frame_buffer->get_color_attachment_renderer_id();
		ImGui::Image((void*)tex_id, ImVec2{ size.x, size.y }, ImVec2{ 0, 1 }, ImVec2{ 1, 0 });

		ImGui::End();

		ImGui::PopStyleVar();



		///////////////////////////Gizmo
		Entity selected_entity = m_scene_hierarchy_panel.GetSelectedEntity();

		m_gizmo_type = 0;
		if (selected_entity && m_gizmo_type != -1)
		{
			ImGuizmo::SetOrthographic(false);
			ImGuizmo::SetDrawlist();

			float window_width = (float)ImGui::GetWindowWidth();
			float window_height = (float)ImGui::GetWindowHeight();
			ImGuizmo::SetRect(ImGui::GetWindowPos().x, ImGui::GetWindowPos().y, window_width, window_height);

			//camera runtime
			/*Entity camera_entity = m_scene->GetPrimaryCameraEntity();
			const auto& camera = camera_entity.GetComponent<CameraComponent>().camera;
			const glm::mat4& camera_projection = camera.get_projection();
			glm::mat4 camera_view = glm::inverse(
					camera_entity.GetComponent<TransformComponent>().get_transformation());*/

			//editor camera
			const glm::mat4& camera_projection = m_editor_camera.get_projection();
			glm::mat4 camera_view = glm::inverse(m_editor_camera.GetViewMatrix());

			//entity transform
			auto& tc = selected_entity.GetComponent<TransformComponent>();
			glm::mat4 transform = tc.get_transformation();

			//Sanaping
			bool snap = Input::IsKeyPress(KeyCode::E_KEY_LEFT_CONTROL);
			float snap_value = 0.5;
			if (m_gizmo_type == ImGuizmo::OPERATION::ROTATE)
				snap_value = 45.0f;

			float snap_values[3] = { snap_value, snap_value, snap_value };

			ImGuizmo::Manipulate(glm::value_ptr(camera_view), glm::value_ptr(camera_projection),
					(ImGuizmo::OPERATION)m_gizmo_type, ImGuizmo::LOCAL, glm::value_ptr(transform), nullptr,
					snap ? snap_values : nullptr);

			if (ImGuizmo::IsUsing())
			{
				glm::vec3 translation, rotation, scale;
				Math::DecomposeTransform(transform, translation, rotation, scale);

				glm::vec3 delta_rotation = rotation - tc.rotation;
				tc.translation = translation;
				tc.rotation += delta_rotation;
				tc.scale = scale;
			}

		}




		/////////////////////////Stats
		auto stats = Engine::Renderer2D::GetStats();
		ImGui::Begin("Settings");

		Application::get()->get_imgui_layer()->set_block_event(!ImGui::IsWindowFocused() && !ImGui::IsWindowHovered());

		ImGui::Text("FPS: %d", (int)m_fps);
		ImGui::Text("draw calls: %d", stats.draw_calls);
		ImGui::Text("quads: %d", stats.quads);
		ImGui::Text("indices: %d", stats.get_indices());
		ImGui::Text("vertices: %d", stats.get_vertices());

		ImGui::End();

		m_scene_hierarchy_panel.OnImGuiRender();

		ImGui::End();
	}

	bool EngineEditorLayer::OnKeyPress(KeyPress& event)
	{
		if (event.get_repeated_count() > 0)
		{ return false; }

		bool control =
				Input::IsKeyPress(KeyCode::E_KEY_LEFT_CONTROL) || Input::IsKeyPress(KeyCode::E_KEY_RIGHT_CONTROL);
		bool shift = Input::IsKeyPress(KeyCode::E_KEY_LEFT_SHIFT) || Input::IsKeyPress(KeyCode::E_KEY_RIGHT_SHIFT);

		switch (event.get_key())
		{
		case (int)KeyCode::E_KEY_N:
		{
			if (control)
			{
				NewScene();
			}
			break;
		}
		case (int)KeyCode::E_KEY_O:
		{
			if (control)
			{
				OpenScene();
			}
			break;
		}
		case (int)KeyCode::E_KEY_S:
		{
			if (control && shift)
			{
				SaveSceneAs();
			}
			break;
		}

			//Gizmo
		case (int)KeyCode::E_KEY_Q:
			m_gizmo_type = -1;
			break;

		case (int)KeyCode::E_KEY_W:
			m_gizmo_type = 0;
			break;

		case (int)KeyCode::E_KEY_E:
			m_gizmo_type = 1;
			break;

		case (int)KeyCode::E_KEY_R:
			m_gizmo_type = 2;
			break;

		}

		return false;
	}
	void EngineEditorLayer::NewScene()
	{
		m_scene = CreateRef<Scene>();
		m_scene->OnViewResize((uint32_t)m_viewport_size.x, (uint32_t)m_viewport_size.y);
		m_scene_hierarchy_panel.set_context(m_scene);
	}
	void EngineEditorLayer::OpenScene()
	{
		std::string file_path = FileDialogs::OpenFile("Engine Scene (*.engine)\0*.engine\0");
		if (!file_path.empty())
		{
			m_scene = CreateRef<Scene>();

			SceneSerializer serializer(m_scene);
			serializer.Deserialize(file_path);

			m_scene->OnViewResize((uint32_t)m_viewport_size.x, (uint32_t)m_viewport_size.y);
			m_scene_hierarchy_panel.set_context(m_scene);
		}
	}
	void EngineEditorLayer::SaveSceneAs()
	{
		std::string file_path = FileDialogs::SaveFile("Engine Scene (*.engine)\0*.engine\0");
		if (!file_path.empty())
		{
			SceneSerializer serializer(m_scene);
			serializer.Serialize(file_path);
		}
	}
}