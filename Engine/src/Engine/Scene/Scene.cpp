#include "pch.h"

#include <box2d/b2_world.h>
#include <box2d/b2_body.h>
#include <box2d/b2_fixture.h>
#include <box2d/b2_polygon_shape.h>

#include "Scene.h"
#include "Entity.h"
#include "Components.h"
#include "Engine/Renderer/Renderer2D.h"
#include "Engine/Renderer/Camera.h"
#include "Engine/Renderer/RendererCommand.h"

namespace Engine
{
	static b2BodyType Rigidbody2DTypeToBox2DType(Rigidbody2DComponent::BodyType type)
	{
		switch (type)
		{
		case Rigidbody2DComponent::BodyType::Static:
			return b2_staticBody;
			break;
		case Rigidbody2DComponent::BodyType::Dynamic:
			return b2_dynamicBody;
			break;
		case Rigidbody2DComponent::BodyType::Kinematic:
			return b2_kinematicBody;
			break;
		}
	}

	Scene::Scene()
	{
	}
	Scene::~Scene()
	{
		if (m_physics_world)delete m_physics_world;
	}

	Entity Scene::CreateEntity(const std::string& tag)
	{
		auto entity = Entity(m_registry.create(), this);
		entity.AddComponent<TransformComponent>();
		entity.AddComponent<TagComponent>(tag);
		return entity;
	}

	void Scene::DestroyEntity(Entity& entity)
	{
		m_registry.destroy(entity);
	}

	void Scene::OnUpdateEditor(TimeStep ts, EditorCamera& camera)
	{
		Renderer2D::BeginScene(camera);

		auto group = m_registry.group<TransformComponent>(entt::get<SpriteRendererComponent>);

		for (auto entity : group)
		{
			auto [transform, sprite_renderer] = m_registry.get<TransformComponent, SpriteRendererComponent>(entity);
			Renderer2D::DrawSprite(transform.get_transformation(), sprite_renderer, (int32_t)entity);
		}

		Renderer2D::EndScene();
	}

	void Scene::OnUpdateRuntime(TimeStep ts)
	{
		//scripts
		{
			m_registry.view<NativeScriptComponent>().each([=](auto entity, auto& nsc)
			{
			  if (!nsc.instance)
			  {
				  nsc.instance = nsc.InstantiateScript();
				  nsc.instance->m_entity = Entity(entity, this);
				  nsc.instance->OnCreate();
			  }
			  nsc.instance->OnUpdate(ts);
			});
		}

		//physics
		{
			const int32_t velocity_iterations = 6;
			const int32_t position_iterations = 2;
			m_physics_world->Step(ts, velocity_iterations, position_iterations);

			auto view = m_registry.view<Rigidbody2DComponent>();

			for (auto e : view)
			{
				Entity entity = { e, this };

				auto& transform = entity.GetComponent<TransformComponent>();
				auto& rb2d = entity.GetComponent<Rigidbody2DComponent>();

				b2Body* body = (b2Body*)rb2d.runtime_body;

				const auto& position = body->GetPosition();
				transform.translation.x = position.x;
				transform.translation.y = position.y;
				transform.rotation.z = body->GetAngle();
			}
		}

		Camera* main_camera{ nullptr };
		glm::mat4 cam_transform;

		auto view = m_registry.view<TransformComponent, CameraComponent>();
		for (auto entity : view)
		{
			auto [transform, cameraComponent] = view.get<TransformComponent, CameraComponent>(entity);

			if (cameraComponent.primary)
			{
				main_camera = &cameraComponent.camera;
				cam_transform = transform.get_transformation();
				break;
			}
		}

		Engine::RendererCommand::SetClearColor({ 0.1f, 0.1f, 0.1f, 0.1f });
		Engine::RendererCommand::Clear();

		if (main_camera)
		{
			Renderer2D::BeginScene(*main_camera, cam_transform);

			auto group = m_registry.group<TransformComponent>(entt::get<SpriteRendererComponent>);

			for (auto entity : group)
			{
				auto [transform, sprite_renderer] = m_registry.get<TransformComponent, SpriteRendererComponent>(entity);
				Renderer2D::DrawQuad(transform.get_transformation(), sprite_renderer.color);
			}

			Renderer2D::EndScene();
		}
		else
		{
			WARN("Scene without camera");
		}

	}

	void Scene::OnViewResize(uint32_t width, uint32_t height)
	{
		m_viewport_width = width;
		m_viewport_height = height;

		auto view = m_registry.view<CameraComponent>();

		for (auto entity : view)
		{
			auto& camera = view.get<CameraComponent>(entity);
			if (!camera.fix_aspect_ratio)
			{
				camera.camera.SetViewport(width, height);
			}
		}
	}

	void Scene::OnRuntimeBegin()
	{
		m_physics_world = new b2World({ 0.0f, -9.8f });

		auto view = m_registry.view<Rigidbody2DComponent>();
		for (auto e : view)
		{
			Entity entity = { e, this };
			auto& transform = entity.GetComponent<TransformComponent>();
			auto& rb2d = entity.GetComponent<Rigidbody2DComponent>();

			b2BodyDef body_def;

			body_def.type = Rigidbody2DTypeToBox2DType(rb2d.type);
			body_def.position = { transform.translation.x, transform.translation.y };
			body_def.angle = transform.rotation.z;
			body_def.fixedRotation = rb2d.fixed_rotation;

			b2Body* body = m_physics_world->CreateBody(&body_def);
			rb2d.runtime_body = body;

			if (entity.HasComponent<BoxCollider2DComponent>())
			{
				auto& bc2d = entity.GetComponent<BoxCollider2DComponent>();

				b2PolygonShape box_shape;

				box_shape.SetAsBox(transform.scale.x * bc2d.size.x, transform.scale.y * bc2d.size.y);

				b2FixtureDef fixture_def;
				fixture_def.shape = &box_shape;

				fixture_def.density = bc2d.density;
				fixture_def.friction = bc2d.friction;
				fixture_def.restitution = bc2d.restitution;
				fixture_def.restitutionThreshold = bc2d.restitution_threshold;
				body->CreateFixture(&fixture_def);
			}
		}
	}

	void Scene::OnRuntimeStop()
	{
		delete m_physics_world;
		m_physics_world = nullptr;
	}

	Entity Scene::GetPrimaryCameraEntity()
	{
		auto view = m_registry.view<CameraComponent>();
		for (auto entity : view)
		{
			const auto& camera = view.get<CameraComponent>(entity);
			if (camera.primary)
			{
				return { entity, this };
			}
		}

		return { entt::null, nullptr };
	}

}