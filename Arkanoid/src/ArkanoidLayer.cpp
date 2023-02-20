#include <PlatformController.h>
#include "ArkanoidLayer.h"
#include "PlayerController.h"
#include "BallController.h"

namespace Arkanoid
{
	ArkanoidLayer::ArkanoidLayer()
			:Layer("Arkanoid")
	{
	}

	void ArkanoidLayer::OnAttach()
	{

		m_platform_texture = Texture2D::Create("../../../Arkanoid/assets/textures/09-Breakout-Tiles.png");

		m_scene = CreateRef<Scene>();

		Entity camera_entity = m_scene->CreateEntity("Main Camera");
		auto& camera_cmp = camera_entity.AddComponent<CameraComponent>();
		camera_cmp.primary = true;
		camera_cmp.camera.SetProjectionType(SceneCamera::ProjectionType::Orthographic);
		camera_cmp.camera.SetOrthographic(1, 1, -1);
		camera_cmp.camera.SetViewport(1280, 720);
		auto& camera = camera_cmp.camera;

		Entity quad = m_scene->CreateEntity("quad");
		quad.AddComponent<SpriteRendererComponent>().texture = Texture2D::Create(
				"../../../Arkanoid/assets/textures/31-Breakout-Tiles.png");
		auto& transform_cmp = quad.GetComponent<TransformComponent>();
		transform_cmp.translation = { 0, -0.5f + 0.02, 0 };
		transform_cmp.scale = { 0.2, 0.04, 1 };

		quad.AddComponent<NativeScriptComponent>().Bind<Scripts::PlayerController>();
		quad.AddComponent<Box2DComponent>();

		Entity ball = m_scene->CreateEntity("Ball");
		TransformComponent& transform_cmp_ball = ball.GetComponent<TransformComponent>();
		transform_cmp_ball.scale = { 0.04f, 0.04f, 1 };
		transform_cmp_ball.translation = { 0, -0.5f + 0.1f, 0 };

		ball.AddComponent<SpriteRendererComponent>().texture = Texture2D::Create(
				"../../../Arkanoid/assets/textures/63-Breakout-Tiles.png");
		const std::array<glm::vec2, 2>& camera_bound = camera.GetOrthographicBound();

		auto& nsc = ball.AddComponent<NativeScriptComponent>();
		nsc.Bind<Scripts::BallController>([&](auto& instance)
		{
		  auto ball_controller = static_cast<Scripts::BallController*>(instance.get());
		  ball_controller->out_of_field += [&]()
		  {
			CreateLevel();
		  };

		});

		ball.AddComponent<Circle2DComponent>();

		float offset = 0.03f;
		for (int i = 0; i < 7; ++i)
		{
			for (int j = 0; j < 8; ++j)
			{
				Entity platform = m_scene->CreateEntity("platform");
				auto& plat_trans_cmp = platform.GetComponent<TransformComponent>();
				plat_trans_cmp.scale = { 0.17, 0.04, 1 };
				plat_trans_cmp.translation = { (camera_bound[0].x + (plat_trans_cmp.scale.x / 2) + 0.1f)
													   + j * (plat_trans_cmp.scale.x + offset),
											   (camera_bound[0].y - (plat_trans_cmp.scale.y / 2, 0) - 0.1f)
													   - i * (plat_trans_cmp.scale.y + offset), 0 };

				platform.AddComponent<SpriteRendererComponent>().texture = m_platform_texture;

				platform.AddComponent<Box2DComponent>();

				platform.AddComponent<NativeScriptComponent>().Bind<Scripts::PlatformController>();
			}

		}

	}

	void ArkanoidLayer::OnUpdate(TimeStep ts)
	{
		RendererCommand::SetClearColor({ 0.1f, 0.1f, 0.1f, 1.0f });
		RendererCommand::Clear();

		m_scene->OnUpdateRuntime(ts);
	}

	void ArkanoidLayer::OnEvent(Event& event)
	{
		EventDispatcher ed(event);
		ed.Dispatch<WindowResized>(BIND_EVENT_FUNC(ArkanoidLayer::OnResize));
	}

	void ArkanoidLayer::OnImGuiRender()
	{

	}

	bool ArkanoidLayer::OnResize(WindowResized& event)
	{
		m_scene->OnViewResize(event.GetWidth(), event.GetHeight());
		return false;
	}

	void ArkanoidLayer::CreateLevel()
	{
		m_scene = CreateRef<Scene>();
	}

}
