#pragma once
#include <Engine.h>

namespace Arkanoid
{
	using namespace Engine;
	class ArkanoidLayer : public Engine::Layer
	{
	public:
		ArkanoidLayer();
		virtual ~ArkanoidLayer() = default;

		virtual void OnAttach() override;

		virtual void OnUpdate(TimeStep ts) override;

		virtual void OnEvent(Event& event) override;

		virtual void OnImGuiRender() override;

	private:
		static constexpr float s_aspect_ration = 1280.0f / 720.f;

		Ref<Texture2D> m_player_platform_texture;
		glm::vec2 m_player_pos;
		glm::vec2 m_player_size{ 0.4, 0.1 };

		Ref<Texture2D> m_ball_texture;
		glm::vec2 m_ball_pos{ 0, 0 };
		glm::vec2 m_ball_size{ 0.07f, 0.07f };
		glm::vec2 m_ball_velocity{ 1, 1 };

		Ref<Texture2D> m_platform_texture;
		glm::vec2 m_platform_size{ 0.4, 0.1 };

		OrthographicCamera m_camera;
	};
}
