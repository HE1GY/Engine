#include "Engine.h"

#include "FirstGame/Quad.h"

class Sandbox2D : public Engine::Layer
{
public:
	Sandbox2D();
	virtual ~Sandbox2D() override = default;

	virtual void OnAttach() override;

	virtual void OnDetach() override;

	virtual void OnUpdate(Engine::TimeStep ts) override;

	virtual void OnEvent(Engine::Event& event) override;

	virtual void OnImGuiRender() override;

private:
	bool Collide(const Quad& quad1, const Quad& quad2);

	Engine::OrthographicCameraController m_camera_controller;
	glm::vec4 m_uniform_color{ 0.2, 0.8, 0.8, 1 };
	Engine::Ref<Engine::Texture> m_texture;

	Quad m_player{{ 0, 0, 0 }, { 0.75, 1 }, 0, { 0.3, 0.2, 0.8, 1 }};
	bool m_jumping{ false };
	glm::vec3 m_velocity{ 0, 0, 0 };
	int m_score{ 0 };

	float m_env_x_pos{ 0 };

	Quad m_ground{{ 0, -4, 0 }, { 20, 0.5f }, 0, { 0.5, 0.6, 0.7, 1 }};
	Quad m_ceiling{{ 0, 4, 0 }, { 20, 0.5f }, 0, { 0.5, 0.6, 0.7, 1 }};

	Quad m_top_pillar{{ 0, -4, 0 }, { 2, 4 }, 0, { 0.5, 0.5, 0.7, 1 }};
	Quad m_bottom_pillar{{ 0, 4, 0 }, { 2, 4 }, 0, { 0.5, 0.5, 0.7, 1 }};

};

