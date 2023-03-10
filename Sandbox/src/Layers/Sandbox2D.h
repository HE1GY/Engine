#include "Engine.h"

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
	glm::vec4 m_uniform_color{ 0.2, 0.8, 0.8, 1 };
	Engine::Ref<Engine::Texture2D> m_texture_chess;
	Engine::Ref<Engine::Texture2D> m_texture_sprites;
	Engine::Ref<Engine::SubTexture2D> m_wall, m_tree;

	float m_fps{ 0 };
};

