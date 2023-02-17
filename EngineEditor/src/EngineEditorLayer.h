#pragma once

#include "Engine.h"
#include "Panel/SceneHierarchyPanel.h"
#include "Engine/Scene/SceneSerializer.h"
#include "Engine/Renderer/EditorCamera.h"

namespace Engine
{
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
		bool OnKeyPress(KeyPress& event);
		void NewScene();
		void OpenScene();
		void SaveSceneAs();

		void DrawDockSpace(std::function<void()> func);
		void DrawStatsWindow();
		void DrawGizmo();
		void DrawViewportWindow();
		void DrawFileMenu();
	private:

		glm::vec2 m_viewport_size{ 0, 0 };
		
		glm::vec2 m_viewport_bound[2]{};

		Ref<FrameBuffer> m_frame_buffer;

		float m_fps{ 0 };

		Ref<Scene> m_scene;

		SceneHierarchyPanel m_scene_hierarchy_panel;
		int m_gizmo_type{ -1 };

		EditorCamera m_editor_camera;
	};

}
