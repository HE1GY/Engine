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
			:Layer("Engine-EditorLayer"), m_scene{ CreateRef<Scene>() }
	{
	}

	void EngineEditorLayer::OnAttach()
	{
		FrameBufferSpecification fb_spec;
		fb_spec.width = 1280;
		fb_spec.height = 720;
		m_frame_buffer = FrameBuffer::Create(fb_spec);

		m_editor_camera = EditorCamera(30.0f, 1.778f, 0.1f, 1000.0f);

		m_scene_hierarchy_panel.SetContext(m_scene);

	}

	void EngineEditorLayer::OnDetach()
	{
	}

	void EngineEditorLayer::OnUpdate(Engine::TimeStep ts)
	{

		Engine::RendererCommand::SetClearColor({ 0.1f, 0.1f, 0.1f, 0.1f });
		Engine::RendererCommand::Clear();

		Engine::Renderer2D::ResetStats();
		m_fps = (float)1 / ts;

		m_frame_buffer->Bind();

		m_editor_camera.OnUpdate(ts);
		m_scene->OnUpdateEditor(ts, m_editor_camera);

		m_frame_buffer->UnBind();
	}

	void EngineEditorLayer::OnEvent(Event& event)
	{
		EventDispatcher ed(event);
		ed.Dispatch<KeyPress>(BIND_EVENT_FUNC(EngineEditorLayer::OnKeyPress));

		m_editor_camera.OnEvent(event);

	}

	void EngineEditorLayer::OnImGuiRender()
	{
		DrawDockSpace([&]()
		{
		  DrawFileMenu();

		  DrawViewportWindow();

		  DrawGizmo();

		  DrawStatsWindow();

		  m_scene_hierarchy_panel.OnImGuiRender();
		});

	}

	void EngineEditorLayer::DrawDockSpace(std::function<void()> func)
	{
		ImGuiWindowFlags window_flags = ImGuiWindowFlags_MenuBar | ImGuiWindowFlags_NoDocking;

		const ImGuiViewport* viewport = ImGui::GetMainViewport();
		ImGui::SetNextWindowPos(viewport->WorkPos);
		ImGui::SetNextWindowSize(viewport->WorkSize);
		ImGui::SetNextWindowViewport(viewport->ID);
		ImGui::PushStyleVar(ImGuiStyleVar_WindowRounding, 0.0f);
		ImGui::PushStyleVar(ImGuiStyleVar_WindowBorderSize, 0.0f);
		window_flags |= ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoResize
				| ImGuiWindowFlags_NoMove;
		window_flags |= ImGuiWindowFlags_NoBringToFrontOnFocus | ImGuiWindowFlags_NoNavFocus;

		ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(0.0f, 0.0f));

		static bool p_open_val = false;
		static bool* p_open = &p_open_val;
		ImGui::Begin("DockSpace ", p_open, window_flags);

		ImGui::PopStyleVar();
		ImGui::PopStyleVar(2);

		ImGuiIO& io = ImGui::GetIO();
		ImGuiStyle& style = ImGui::GetStyle();

		float prev_window_min_size = style.WindowMinSize.x;
		style.WindowMinSize.x = 370.0f;

		if (io.ConfigFlags & ImGuiConfigFlags_DockingEnable)
		{
			ImGuiID dockspace_id = ImGui::GetID("MyDockSpace");
			ImGui::DockSpace(dockspace_id, ImVec2(0.0f, 0.0f));
		}
		style.WindowMinSize.x = prev_window_min_size;

		func();

		ImGui::End();
	}

	void EngineEditorLayer::DrawStatsWindow()
	{
		auto stats = Engine::Renderer2D::GetStats();
		ImGui::Begin("Stats");

		ImGui::Text("FPS: %d", (int)m_fps);
		ImGui::Text("draw calls: %d", stats.draw_calls);
		ImGui::Text("quads: %d", stats.quads);
		ImGui::Text("indices: %d", stats.get_indices());
		ImGui::Text("vertices: %d", stats.get_vertices());

		ImGui::End();
	}

	void EngineEditorLayer::DrawGizmo()
	{
		Entity selected_entity = m_scene_hierarchy_panel.GetSelectedEntity();

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
	}

	void EngineEditorLayer::DrawViewportWindow()
	{
		ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(0, 0));

		ImGui::Begin("Viewport");
		Application::get()->get_imgui_layer()->set_block_event(!ImGui::IsWindowFocused() && !ImGui::IsWindowHovered());

		ImVec2 size = ImGui::GetContentRegionAvail();

		if (size.x != m_viewport_size.x || size.y != m_viewport_size.y)
		{
			m_viewport_size = { size.x, size.y };
			m_frame_buffer->Resize(size.x, size.y);
			m_scene->OnViewResize(size.x, size.y);
			m_editor_camera.SetViewportSize(size.x, size.y);
		}

		uint32_t tex_id = m_frame_buffer->get_color_attachment_renderer_id();
		ImGui::Image((void*)tex_id, ImVec2{ size.x, size.y }, ImVec2{ 0, 1 }, ImVec2{ 1, 0 });

		ImGui::End();

		ImGui::PopStyleVar();
	}

	void EngineEditorLayer::DrawFileMenu()
	{
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
	}
	//
	bool EngineEditorLayer::OnKeyPress(KeyPress& event)
	{
		//Shortcuts


		if (event.get_repeated_count() > 0)
		{
			return false;
		}

		bool control =
				Input::IsKeyPress(KeyCode::E_KEY_LEFT_CONTROL) || Input::IsKeyPress(KeyCode::E_KEY_RIGHT_CONTROL);
		bool shift = Input::IsKeyPress(KeyCode::E_KEY_LEFT_SHIFT) || Input::IsKeyPress(KeyCode::E_KEY_RIGHT_SHIFT);

		//File menu
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
		m_scene.reset();
		m_scene = CreateRef<Scene>();
		m_scene->OnViewResize((uint32_t)m_viewport_size.x, (uint32_t)m_viewport_size.y);
		m_scene_hierarchy_panel.SetContext(m_scene);
	}

	void EngineEditorLayer::OpenScene()
	{
		std::string file_path;
#ifdef WINDOWS
		file_path = FileDialogs::OpenFile("Engine Scene (*.engine)\0*.engine\0");
#endif
#ifdef LINUX
		file_path = FileDialogs::OpenFile("*.engine");
#endif

		if (!file_path.empty())
		{
			m_scene = CreateRef<Scene>();

			SceneSerializer serializer(m_scene);
			serializer.Deserialize(file_path);

			m_scene->OnViewResize((uint32_t)m_viewport_size.x, (uint32_t)m_viewport_size.y);
			m_scene_hierarchy_panel.SetContext(m_scene);
		}
	}

	void EngineEditorLayer::SaveSceneAs()
	{
		std::string file_path;
#ifdef WINDOWS
		file_path = FileDialogs::OpenFile("Engine Scene (*.engine)\0*.engine\0");
#endif
#ifdef LINUX
		file_path = FileDialogs::SaveFile("*.engine");
#endif
		if (!file_path.empty())
		{
			SceneSerializer serializer(m_scene);
			serializer.Serialize(file_path);
		}
	}

}