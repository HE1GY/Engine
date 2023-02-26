#include "pch.h"

#include <glm/ext/matrix_clip_space.hpp>
#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>

#include "config.h"
#include "Engine/Utils/FileDialogs.h"
#include "EngineEditorLayer.h"

#include "Engine/Scene/SceneSerializer.h"

#include <ImGuizmo.h>

#include "Engine/Math/Math.h"

namespace Engine
{
	EngineEditorLayer::EngineEditorLayer()
			:Layer("Engine-EditorLayer"), m_active_scene{ CreateRef<Scene>() }
	{
	}

	void EngineEditorLayer::OnAttach()
	{
		FrameBufferSpecification fb_spec;
		fb_spec.attachment_specification = { FrameBufferTextureFormat::RGBA8, FrameBufferTextureFormat::RED_INTEGER,
											 FrameBufferTextureFormat::Depth };
		fb_spec.width = 1280;
		fb_spec.height = 720;
		m_frame_buffer = FrameBuffer::Create(fb_spec);
		m_editor_camera = EditorCamera(30.0f, 1.778f, 0.1f, 1000.0f);

		m_scene_hierarchy_panel.SetContext(m_active_scene);

		m_play_icon = Texture2D::Create(CMAKE_SOURCE_DIR"/EngineEditor/resources/icon/PlayButton.png");
		m_stop_icon = Texture2D::Create(CMAKE_SOURCE_DIR"/EngineEditor/resources/icon/PauseButton.png");
	}

	void EngineEditorLayer::OnDetach()
	{
	}

	void EngineEditorLayer::OnUpdate(Engine::TimeStep ts)
	{
		Engine::Renderer2D::ResetStats();
		m_fps = (float)1 / ts;

		m_frame_buffer->Bind();
		Engine::RendererCommand::SetClearColor({ 0.1f, 0.1f, 0.1f, 0.1f });
		Engine::RendererCommand::Clear();
		m_frame_buffer->ClearAttachment(1, -1);

		switch (m_scene_state)
		{
		case SceneState::Play:
		{
			m_active_scene->OnUpdateRuntime(ts);
			break;
		}
		case SceneState::Edit:
		{
			m_editor_camera.OnUpdate(ts, m_viewport_hovered);
			m_active_scene->OnUpdateEditor(ts, m_editor_camera);
			break;
		}
		}

		OnOverlayRender();

		auto [mx, my] = ImGui::GetMousePos();
		mx -= m_viewport_bound[0].x;
		my -= m_viewport_bound[0].y;

		my = m_viewport_size.y - my;//flip y

		int mouse_x = (int)mx;
		int mouse_y = (int)my;

		if (mouse_x >= 0 && mouse_y >= 0 && mouse_x < (int)m_viewport_size.x && mouse_y < (int)m_viewport_size.y)
		{
			int data = m_frame_buffer->ReadPixel(1, mouse_x, mouse_y);
			if (data != -1)
			{
				m_hovered_entity = Entity((entt::entity)data, m_active_scene.get());
			}
			else
			{
				m_hovered_entity = Entity(entt::null, nullptr);
			}
		}

		m_frame_buffer->UnBind();
	}

	void EngineEditorLayer::OnEvent(Event& event)
	{
		EventDispatcher ed(event);
		ed.Dispatch<KeyPress>(BIND_EVENT_FUNC(EngineEditorLayer::OnKeyPress));
		ed.Dispatch<MouseButtonPressed>(BIND_EVENT_FUNC(EngineEditorLayer::OnMouseButtonPress));

		m_editor_camera.OnEvent(event);
	}

	void EngineEditorLayer::OnOverlayRender()
	{

		switch (m_scene_state)
		{
		case SceneState::Play:
		{
			auto camera_entity = m_active_scene->GetPrimaryCameraEntity();
			if (camera_entity)
			{
				Renderer2D::BeginScene(camera_entity.GetComponent<CameraComponent>().camera.GetProjection(),
						glm::inverse(camera_entity.GetComponent<TransformComponent>().get_transformation()));
			}
			break;
		}
		case SceneState::Edit:
		{
			Renderer2D::BeginScene(m_editor_camera.GetProjection(), m_editor_camera.GetViewMatrix());
			break;
		}
		}

		//Colliders
		if (m_show_colliders)
		{
			{
				auto view = m_active_scene->GetAllEntitiesWith<TransformComponent, CircleCollider2DComponent>();

				for (auto entity : view)
				{
					const auto [tr_cmp, cc2d_cmp] = view.get<TransformComponent, CircleCollider2DComponent>(entity);

					glm::mat4 transformation = glm::translate(glm::mat4(1.0f),
							{ tr_cmp.translation.x + cc2d_cmp.offset.x,
							  tr_cmp.translation.y + cc2d_cmp.offset.y,
							  tr_cmp.translation.z + 0.05f })
							* glm::rotate(glm::mat4(1.0f), tr_cmp.rotation.z, { 0, 0, 1 })
							* glm::scale(glm::mat4(1.0f), cc2d_cmp.radius * tr_cmp.scale * 2.1f);

					Renderer2D::DrawCircle(transformation, { 0, 1, 0, 1 }, 0.05f, 0.005f, (int32_t)entity);
				}
			}

			{
				auto view = m_active_scene->GetAllEntitiesWith<TransformComponent, BoxCollider2DComponent>();

				for (auto entity : view)
				{
					const auto [tr_cmp, bc2d_cmp] = view.get<TransformComponent, BoxCollider2DComponent>(entity);

					glm::mat4 transformation = glm::translate(glm::mat4(1.0f),
							{ tr_cmp.translation.x + bc2d_cmp.offset.x, tr_cmp.translation.y + bc2d_cmp.offset.y,
							  tr_cmp.translation.z + 0.05f })
							* glm::rotate(glm::mat4(1.0f), tr_cmp.rotation.z, { 0, 0, 1 })
							* glm::scale(glm::mat4(1.0f),
									glm::vec3{ bc2d_cmp.size.x, bc2d_cmp.size.y, 1 } * tr_cmp.scale * 2.0f);

					Renderer2D::DrawRect(transformation, { 0, 1, 0, 1 }, 3.0f, (int32_t)entity);
				}
			}
		}
		{
			//Camera frustum
			if (m_scene_state == SceneState::Edit)
			{

				auto cam_entity = m_active_scene->GetPrimaryCameraEntity();
				if (cam_entity)
				{
					auto& cam_cmp = cam_entity.GetComponent<CameraComponent>();
					if (cam_cmp.camera.GetProjectionType() == ProjectionType::Orthographic)
					{
						auto& transform_cmp = cam_entity.GetComponent<TransformComponent>();

						std::array<glm::vec2, 2> bound = cam_cmp.camera.GetOrthographicBound();

						glm::mat4 transform = transform_cmp.get_transformation()
								* glm::translate(glm::mat4(1.0f), { 0, 0, 0.5f })
								* glm::scale(glm::mat4(1.0f), { bound[1].x * 2, bound[0].y * 2, 0 });

						Renderer2D::DrawRect(transform, { 0.8, 0.8, 0.8, 1 }, 3.0f, (int32_t)cam_entity);

						/*Ref<Texture> tex;
						Renderer2D::DrawQuad(transform, tex, { 0.8, 0.8, 0.8, 0.4 }, (int32_t)cam_entity);*/
					}
				}
			}
		}
		Renderer2D::EndScene();
	}

	void EngineEditorLayer::OnImGuiRender()
	{
		DrawDockSpace([&]()
		{
		  DrawFileMenu();

		  DrawViewportWindow();

		  DrawSettingsWindow();

		  DrawToolbar();

		  m_scene_hierarchy_panel.OnImGuiRender();
		  m_content_panel.OnImGuiRender();
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

	void EngineEditorLayer::DrawSettingsWindow()
	{
		auto stats = Engine::Renderer2D::GetStats();
		ImGui::Begin("Stats");

		ImGui::Text("FPS: %d", (int)m_fps);
		ImGui::Text("draw calls: %d", stats.draw_calls);
		ImGui::Text("quads: %d", stats.quads);
		ImGui::Text("circle: %d", stats.circles);
		ImGui::Text("lines: %d", stats.lines);
		ImGui::Text("indices: %d", stats.GetIndices());
		ImGui::Text("vertices: %d", stats.GetVertices());

		if (m_hovered_entity)
		{
			if (m_hovered_entity.HasComponent<TagComponent>())
			{
				ImGui::Text("Hovered entity: %s  id=%d", m_hovered_entity.GetComponent<TagComponent>().tag.c_str(),
						(int32_t)m_hovered_entity);
			}
		}
		else
		{
			ImGui::Text("Hovered entity: Empty");
		}

		ImGui::End();

		ImGui::Begin("Settings");

		ImGui::Checkbox("Show Colliders", &m_show_colliders);

		ImGui::End();
	}

	void EngineEditorLayer::DrawGizmo()
	{

		Entity selected_entity = m_scene_hierarchy_panel.GetSelectedEntity();

		if (selected_entity)
		{
			ImGuizmo::SetOrthographic(true);
			ImGuizmo::SetDrawlist();

			float window_width = (float)ImGui::GetWindowWidth();
			float window_height = (float)ImGui::GetWindowHeight();
			ImGuizmo::SetRect(ImGui::GetWindowPos().x, ImGui::GetWindowPos().y, window_width, window_height);


			//editor camera
			const glm::mat4& camera_projection = m_editor_camera.GetProjection();
			glm::mat4 camera_view = m_editor_camera.GetViewMatrix();

			//entity transform
			auto& tc = selected_entity.GetComponent<TransformComponent>();
			glm::mat4 transform = tc.get_transformation();

			//Sanaping
			bool snap = Input::IsKeyPress(KeyCode::E_KEY_LEFT_CONTROL);
			float snap_value = 0.5;
			if (m_gizmo_type == GizmoType::ROTATE)
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

		m_viewport_hovered = ImGui::IsWindowHovered();
		ImVec2 size = ImGui::GetContentRegionAvail();

		auto viewportMinRegion = ImGui::GetWindowContentRegionMin();
		auto viewportMaxRegion = ImGui::GetWindowContentRegionMax();
		auto viewportOffset = ImGui::GetWindowPos();
		m_viewport_bound[0] = { viewportMinRegion.x + viewportOffset.x, viewportMinRegion.y + viewportOffset.y };
		m_viewport_bound[1] = { viewportMaxRegion.x + viewportOffset.x, viewportMaxRegion.y + viewportOffset.y };

		if (size.x != m_viewport_size.x || size.y != m_viewport_size.y)
		{
			m_viewport_size = { size.x, size.y };
			m_frame_buffer->Resize(size.x, size.y);
			m_active_scene->OnViewResize(size.x, size.y);
			m_editor_camera.SetViewportSize(size.x, size.y);
		}

		uint32_t tex_id = m_frame_buffer->GetColorAttachmentRendererId(0);
		ImGui::Image((ImTextureID)static_cast<uint64_t>(tex_id), ImVec2{ size.x, size.y }, ImVec2{ 0, 1 },
				ImVec2{ 1, 0 });

		if (m_scene_state == SceneState::Edit)
		{
			DrawGizmo();
		}

		if (ImGui::BeginDragDropTarget())
		{
			if (const ImGuiPayload* payload = ImGui::AcceptDragDropPayload("DND_SCENE"))
			{
				std::filesystem::path scene_path = (const char*)payload->Data;

				OpenScene(scene_path);
			}
			ImGui::EndDragDropTarget();
		}

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

		if (event.GetRepeatedCount() > 0)
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
			if (control)
			{
				if (shift)
				{
					SaveSceneAs();
				}
				else
				{
					SaveScene();
				}
			}
			break;
		}

			//Gizmo
		case (int)KeyCode::E_KEY_Q:
			m_gizmo_type = GizmoType::NONE;
			break;

		case (int)KeyCode::E_KEY_W:
			m_gizmo_type = GizmoType::TRANSLATE;
			break;

		case (int)KeyCode::E_KEY_E:
			m_gizmo_type = GizmoType::ROTATE;
			break;

		case (int)KeyCode::E_KEY_R:
			m_gizmo_type = GizmoType::SCALE;
			break;


			//Enitity
		case (int)KeyCode::E_KEY_D:
		{
			if (control)
			{
				Entity entity = m_scene_hierarchy_panel.GetSelectedEntity();
				if (entity)
				{
					m_active_scene->CopyEntity(entity);
				}
			}
			break;
		}

		}

		return false;
	}

	void EngineEditorLayer::NewScene()
	{
		m_active_scene = CreateRef<Scene>();

		m_active_scene->OnViewResize((uint32_t)m_viewport_size.x, (uint32_t)m_viewport_size.y);
		m_scene_hierarchy_panel.SetContext(m_active_scene);

		m_editor_scene_path = std::filesystem::path();
	}

	void EngineEditorLayer::OpenScene()
	{
		if (m_scene_state != SceneState::Edit)
		{
			OnSceneStop();
		}

		std::string file_path;
		file_path = FileDialogs::OpenFile("*.engine");

		OpenScene(file_path);
	}

	void EngineEditorLayer::OpenScene(std::filesystem::path path)
	{
		if (!path.empty())
		{
			m_active_scene = CreateRef<Scene>();

			SceneSerializer serializer(m_active_scene);
			serializer.Deserialize(path.string());

			m_active_scene->OnViewResize((uint32_t)m_viewport_size.x, (uint32_t)m_viewport_size.y);
			m_scene_hierarchy_panel.SetContext(m_active_scene);
			m_editor_scene_path = path;
			m_hovered_entity = { entt::null, nullptr };
		}
	}

	void EngineEditorLayer::SaveSceneAs()
	{
		std::string file_path;

		file_path = FileDialogs::SaveFile("*.engine");

		if (!file_path.empty())
		{
			SceneSerializer serializer(m_active_scene);
			serializer.Serialize(file_path);

			m_editor_scene_path = file_path;
		}
	}

	void EngineEditorLayer::SaveScene()
	{
		if (!m_editor_scene_path.empty())
		{
			SceneSerializer serializer(m_active_scene);
			serializer.Serialize(m_editor_scene_path.string());
		}
		else
		{
			SaveSceneAs();
		}
	}

	bool EngineEditorLayer::OnMouseButtonPress(MouseButtonPressed& event)
	{
		if (event.get_key() == (int)MouseButtonCode::E_MOUSE_BUTTON_LEFT)
		{
			if (m_viewport_hovered && !ImGuizmo::IsOver())
			{
				m_scene_hierarchy_panel.SetSelectedEntity(m_hovered_entity);
			}
		}

		return false;
	}

	void EngineEditorLayer::DrawToolbar()
	{
		ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(0, 2));
		ImGui::PushStyleVar(ImGuiStyleVar_ItemInnerSpacing, ImVec2(0, 0));
		ImGui::PushStyleColor(ImGuiCol_Button, ImVec4(0, 0, 0, 0));
		auto& colors = ImGui::GetStyle().Colors;
		const auto& buttonHovered = colors[ImGuiCol_ButtonHovered];
		ImGui::PushStyleColor(ImGuiCol_ButtonHovered, ImVec4(buttonHovered.x, buttonHovered.y, buttonHovered.z, 0.5f));
		const auto& buttonActive = colors[ImGuiCol_ButtonActive];
		ImGui::PushStyleColor(ImGuiCol_ButtonActive, ImVec4(buttonActive.x, buttonActive.y, buttonActive.z, 0.5f));

		ImGui::Begin("##toolbar", nullptr,
				ImGuiWindowFlags_NoDecoration | ImGuiWindowFlags_NoScrollbar | ImGuiWindowFlags_NoScrollWithMouse);

		bool toolbarEnabled = (bool)m_active_scene;

		ImVec4 tintColor = ImVec4(1, 1, 1, 1);
		if (!toolbarEnabled)
			tintColor.w = 0.5f;

		float size = ImGui::GetWindowHeight() - 4.0f;
		ImGui::SetCursorPosX((ImGui::GetWindowContentRegionMax().x * 0.5f) - (size * 0.5f));

		bool hasPlayButton = m_scene_state == SceneState::Edit || m_scene_state == SceneState::Play;
		//bool hasSimulateButton = m_scene_state == SceneState::Edit || m_scene_state == SceneState::Simulate;
		//bool hasPauseButton = m_scene_state != SceneState::Edit;

		if (hasPlayButton)
		{
			Ref<Texture2D> icon = (m_scene_state == SceneState::Edit /*|| m_scene_state == SceneState::Simulate*/)
								  ? m_play_icon : m_stop_icon;
			if (ImGui::ImageButton((ImTextureID)(uint64_t)icon->GetRendererId(), ImVec2(size, size), ImVec2(0, 0),
					ImVec2(1, 1), 0, ImVec4(0.0f, 0.0f, 0.0f, 0.0f), tintColor) && toolbarEnabled)
			{
				if (m_scene_state == SceneState::Edit /*|| m_scene_state == SceneState::Simulate*/)
					OnScenePlay();
				else if (m_scene_state == SceneState::Play)
					OnSceneStop();
			}
		}

		/*if (hasSimulateButton)
		{
			if (hasPlayButton)
				ImGui::SameLine();

			Ref<Texture2D> icon = (m_scene_state == SceneState::Edit || m_scene_state == SceneState::Play)
								  ? m_IconSimulate : m_IconStop;
			if (ImGui::ImageButton((ImTextureID)(uint64_t)icon->GetRendererID(), ImVec2(size, size), ImVec2(0, 0),
					ImVec2(1, 1), 0, ImVec4(0.0f, 0.0f, 0.0f, 0.0f), tintColor) && toolbarEnabled)
			{
				if (m_scene_state == SceneState::Edit || m_scene_state == SceneState::Play)
					OnSceneSimulate();
				else if (m_scene_state == SceneState::Simulate)
					OnSceneStop();
			}
		}*/
		/*if (hasPauseButton)
		{
			bool isPaused = m_active_scene->IsPaused();
			ImGui::SameLine();
			{
				Ref<Texture2D> icon = m_IconPause;
				if (ImGui::ImageButton((ImTextureID)(uint64_t)icon->GetRendererID(), ImVec2(size, size), ImVec2(0, 0),
						ImVec2(1, 1), 0, ImVec4(0.0f, 0.0f, 0.0f, 0.0f), tintColor) && toolbarEnabled)
				{
					m_active_scene->SetPaused(!isPaused);
				}
			}

			// Step button
			if (isPaused)
			{
				ImGui::SameLine();
				{
					Ref<Texture2D> icon = m_IconStep;
					bool isPaused = m_active_scene->IsPaused();
					if (ImGui::ImageButton((ImTextureID)(uint64_t)icon->GetRendererID(), ImVec2(size, size),
							ImVec2(0, 0), ImVec2(1, 1), 0, ImVec4(0.0f, 0.0f, 0.0f, 0.0f), tintColor) && toolbarEnabled)
					{
						m_active_scene->Step();
					}
				}
			}
		}
		*/ImGui::PopStyleVar(2);
		ImGui::PopStyleColor(3);
		ImGui::End();
	}

	void EngineEditorLayer::OnScenePlay()
	{
		m_editor_scene = m_active_scene;
		m_active_scene = Scene::Copy(m_editor_scene);
		m_scene_hierarchy_panel.SetContext(m_active_scene);

		m_scene_state = SceneState::Play;
		m_active_scene->OnRuntimeBegin();
	}

	void EngineEditorLayer::OnSceneStop()
	{
		m_active_scene = m_editor_scene;
		m_scene_hierarchy_panel.SetContext(m_active_scene);

		m_scene_state = SceneState::Edit;
		m_active_scene->OnRuntimeStop();
	}

}