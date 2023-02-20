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
		void CreateLevel();
		bool OnResize(WindowResized& event);
	private:
		Ref<Texture2D> m_platform_texture;

		Ref<Scene> m_scene;
		bool m_run{ true };
	};
}
