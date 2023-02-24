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
		const std::array<glm::vec2, 2>& camera_bound = camera.GetOrthographicBound();

		{
			Entity quad = m_scene->CreateEntity("quad");
			quad.AddComponent<SpriteRendererComponent>().texture = Texture2D::Create(
					"../../../Arkanoid/assets/textures/31-Breakout-Tiles.png");

			auto place_player = [quad]()
			{
			  auto& transform_cmp = quad.GetComponent<TransformComponent>();
			  transform_cmp.scale = { 0.2, 0.05, 1 };
			  transform_cmp.translation = { 0, -0.5 + transform_cmp.scale.y / 2, 0 };
			};
			m_restart_level += place_player;

			place_player();

			quad.AddComponent<NativeScriptComponent>().Bind<Scripts::PlayerController>();
			//	quad.AddComponent<Box2DComponent>();
		}

		{
			Entity ball = m_scene->CreateEntity("Ball");

			auto place_ball = [ball]()
			{
			  TransformComponent& transform_cmp_ball = ball.GetComponent<TransformComponent>();
			  transform_cmp_ball.scale = { 0.04f, 0.04f, 1 };
			  transform_cmp_ball.translation = { 0, 0, 0 };
			};
			m_restart_level += place_ball;
			place_ball();

			ball.AddComponent<SpriteRendererComponent>().texture = Texture2D::Create(
					"../../../Arkanoid/assets/textures/63-Breakout-Tiles.png");

			auto& nsc = ball.AddComponent<NativeScriptComponent>();
			nsc.Bind<Scripts::BallController>([&](auto& instance)
			{
			  auto ball_controller = static_cast<Scripts::BallController*>(instance.get());
			  ball_controller->out_of_field += [&]()
			  {
				m_restart_level.Invoke();
			  };
			});

			//ball.AddComponent<Circle2DComponent>();
		}

		for (int i = 0; i < 7; ++i)
		{
			for (int j = 0; j < 8; ++j)
			{
				Entity platform = m_scene->CreateEntity("platform");
				auto place_platform = [platform, camera_bound, i, j]()
				{
				  float offset = 0.03f;
				  auto& plat_trans_cmp = platform.GetComponent<TransformComponent>();
				  plat_trans_cmp.scale = { 0.17, 0.04, 1 };
				  plat_trans_cmp.translation = { (camera_bound[0].x + (plat_trans_cmp.scale.x / 2) + 0.1f)
														 + j * (plat_trans_cmp.scale.x + offset),
												 (camera_bound[0].y - (plat_trans_cmp.scale.y / 2, 0) - 0.1f)
														 - i * (plat_trans_cmp.scale.y + offset), 0 };
				};
				m_restart_level += place_platform;
				place_platform();

				platform.AddComponent<SpriteRendererComponent>().texture = m_platform_texture;

				//	platform.AddComponent<Box2DComponent>();

				platform.AddComponent<NativeScriptComponent>().Bind<Scripts::PlatformController>();
			}

		}

		m_scene->OnRuntimeBegin();
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

}
