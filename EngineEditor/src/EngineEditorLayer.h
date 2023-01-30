#pragma once

#include "Engine.h"
#include "Panel/SceneHierarchyPanel.h"

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
		Engine::OrthographicCameraController m_camera_controller;
		glm::vec4 m_uniform_color{ 0.2, 0.8, 0.8, 1 };
		Engine::Ref<Engine::Texture2D> m_texture_chess;
		Engine::Ref<Engine::Texture2D> m_texture_sprites;
		Engine::Ref<Engine::SubTexture2D> m_wall, m_tree;

		glm::vec2 m_viewport_size{ 0, 0 };

		Ref<FrameBuffer> m_frame_buffer;

		float m_fps{ 0 };

		ParticlesSystem m_particles;

		Ref<Scene> m_scene;
		Entity m_main_cam;

		SceneHierarchyPanel m_scene_hierarchy;
	};

}
