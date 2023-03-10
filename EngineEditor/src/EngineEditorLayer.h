#pragma once

#include <filesystem>

#include "Engine.h"
#include "Panel/SceneHierarchyPanel.h"
#include "Engine/Scene/SceneSerializer.h"
#include "Engine/Scene/EditorCamera.h"
#include "Panel/ContentPanel.h"

namespace Engine
{
	enum class GizmoType
	{
		NONE = -1,
		TRANSLATE = 0,
		ROTATE = 1,
		SCALE = 2,
	};

	class EngineEditorLayer : public Layer
	{
	public:
		EngineEditorLayer();
		~EngineEditorLayer() = default;

		virtual void OnAttach() override;

		virtual void OnDetach() override;

		virtual void OnUpdate(Engine::TimeStep ts) override;

		virtual void OnEvent(Engine::Event& event) override;

		virtual void OnImGuiRender() override;

	private:
		enum class SceneState
		{
			Play, Edit,
		};
	private:
		void OnOverlayRender();

		bool OnKeyPress(KeyPress& event);
		bool OnMouseButtonPress(MouseButtonPressed& event);
		void NewScene();
		void OpenScene();
		void OpenScene(std::filesystem::path path);
		void SaveSceneAs();
		void SaveScene();
		void OnScenePlay();
		void OnSceneStop();

		void DrawDockSpace(std::function<void()> func);
		void DrawSettingsWindow();
		void DrawGizmo();
		void DrawViewportWindow();
		void DrawFileMenu();
		void DrawToolbar();
	private:

		glm::vec2 m_viewport_size{ 0, 0 };
		glm::vec2 m_viewport_bound[2]{};

		Ref<FrameBuffer> m_frame_buffer;

		float m_fps{ 0 };

		Ref<Scene> m_editor_scene;
		Ref<Scene> m_active_scene;

		SceneHierarchyPanel m_scene_hierarchy_panel;
		ContentPanel m_content_panel;

		GizmoType m_gizmo_type{ GizmoType::TRANSLATE };

		EditorCamera m_editor_camera;

		Entity m_hovered_entity;
		bool m_viewport_hovered{ false };

		SceneState m_scene_state{ SceneState::Edit };
		Ref<Texture2D> m_play_icon;
		Ref<Texture2D> m_stop_icon;

		std::filesystem::path m_editor_scene_path;

		bool m_show_colliders{ false };
	};

}
