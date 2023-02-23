#include "pch.h"

#include <box2d/b2_world.h>
#include <box2d/b2_body.h>
#include <box2d/b2_fixture.h>
#include <box2d/b2_polygon_shape.h>
#include <box2d/b2_circle_shape.h>

#include "Scene.h"
#include "Entity.h"
#include "Components.h"
#include "Engine/Renderer/Renderer2D.h"
#include "Engine/Renderer/Camera.h"
#include "Engine/Renderer/RendererCommand.h"
#include "ScriptableEntity.h"

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

	template<typename Component>
	static void CopyComponent(Entity& dst, const Entity& src)
	{
		if (src.HasComponent<Component>())
		{
			auto& component = src.GetComponent<Component>();
			dst.AddOrReplaceComponent<Component>(component);
		}
	}

	template<typename Component>
	static void CopyComponent(entt::registry& dst, entt::registry& src,
			const std::unordered_map<UUID, entt::entity>& dst_entity_ids)
	{
		auto view_component = src.view<Component>();
		for (auto e : view_component)
		{
			auto& id_cmp = src.get<IDComponent>(e);
			auto& cmp = src.get<Component>(e);
			entt::entity dst_entity = dst_entity_ids.at(id_cmp.uuid);
			dst.emplace_or_replace<Component>(dst_entity, cmp);
		}
	}

	Scene::Scene()
	{
	}
	Scene::~Scene()
	{
		if (m_physics_world)delete m_physics_world;
	}

	Ref<Scene> Scene::Copy(Ref<Scene> scene)
	{
		Ref<Scene> new_scene = CreateRef<Scene>();
		new_scene->m_viewport_height = scene->m_viewport_height;
		new_scene->m_viewport_width = scene->m_viewport_width;

		auto& src_registry = scene->m_registry;
		auto& dst_registry = new_scene->m_registry;

		auto view_ids = src_registry.view<IDComponent>();

		std::unordered_map<UUID, entt::entity> dst_entity_ids;

		for (auto e : view_ids)
		{
			UUID id = view_ids.get<IDComponent>(e).uuid;
			std::string tag = src_registry.get<TagComponent>(e).tag;
			dst_entity_ids[id] = new_scene->CreateEntityWithUUID(id, tag);
		}

		//CopyComponent
		CopyComponent<TransformComponent>(dst_registry, src_registry, dst_entity_ids);
		CopyComponent<CameraComponent>(dst_registry, src_registry, dst_entity_ids);
		CopyComponent<SpriteRendererComponent>(dst_registry, src_registry, dst_entity_ids);
		CopyComponent<CircleRendererComponent>(dst_registry, src_registry, dst_entity_ids);
		CopyComponent<Rigidbody2DComponent>(dst_registry, src_registry, dst_entity_ids);
		CopyComponent<BoxCollider2DComponent>(dst_registry, src_registry, dst_entity_ids);
		CopyComponent<CircleCollider2DComponent>(dst_registry, src_registry, dst_entity_ids);
		CopyComponent<NativeScriptComponent>(dst_registry, src_registry, dst_entity_ids);

		return new_scene;
	}

	Entity Scene::CreateEntity(const std::string& tag)
	{
		auto entity = Entity(m_registry.create(), this);
		entity.AddComponent<IDComponent>();
		entity.AddComponent<TransformComponent>();
		entity.AddComponent<TagComponent>(tag);
		return entity;
	}

	Entity Scene::CreateEntityWithUUID(UUID uuid, const std::string& tag)
	{
		auto entity = Entity(m_registry.create(), this);
		entity.AddComponent<IDComponent>(uuid);
		entity.AddComponent<TransformComponent>();
		entity.AddComponent<TagComponent>(tag);
		return entity;
	}

	Entity Scene::CopyEntity(const Entity& entity)
	{
		std::string entity_name = entity.GetName();
		Entity new_entity = CreateEntity(entity_name);

		CopyComponent<TransformComponent>(new_entity, entity);
		CopyComponent<CameraComponent>(new_entity, entity);
		CopyComponent<SpriteRendererComponent>(new_entity, entity);
		CopyComponent<CircleRendererComponent>(new_entity, entity);
		CopyComponent<Rigidbody2DComponent>(new_entity, entity);
		CopyComponent<BoxCollider2DComponent>(new_entity, entity);
		CopyComponent<CircleCollider2DComponent>(new_entity, entity);
		CopyComponent<NativeScriptComponent>(new_entity, entity);

		return new_entity;
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
			//Renderer2D::DrawRect(transform.get_transformation(), { 1, 1, 0, 1 }, 0);
		}

		auto view = m_registry.view<TransformComponent, CircleRendererComponent>();
		for (auto entity : view)
		{
			auto [transform, circle_renderer] = m_registry.get<TransformComponent, CircleRendererComponent>(entity);
			Renderer2D::DrawCircle(transform.get_transformation(), circle_renderer, (int32_t)entity);
		}

		/*Renderer2D::SetLineWidth(2.0f);
		Renderer2D::DrawLine({ 0, 0, 0 }, glm::vec3(5.0f), { 1, 0, 1, 1 }, 0);

		Renderer2D::DrawRect({ 0, 0, 0 }, { 1, 1 }, { 1, 1, 1, 1 }, 0);

		glm::mat4 rect = glm::rotate(glm::mat4(1.0f), glm::radians(45.0f), { 0, 0, 1 });
		Renderer2D::DrawRect(rect, { 1, 1, 0, 1 }, 0);*/

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
				Renderer2D::DrawSprite(transform.get_transformation(), sprite_renderer, (int32_t)entity);
			}

			auto view = m_registry.view<TransformComponent, CircleRendererComponent>();
			for (auto entity : view)
			{
				auto [transform, circle_renderer] = m_registry.get<TransformComponent, CircleRendererComponent>(entity);
				Renderer2D::DrawCircle(transform.get_transformation(), circle_renderer, (int32_t)entity);
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

			if (entity.HasComponent<CircleCollider2DComponent>())
			{
				auto& cc2d = entity.GetComponent<CircleCollider2DComponent>();

				b2CircleShape circle_shape;

				circle_shape.m_radius = cc2d.radius;
				circle_shape.m_p = { cc2d.offset.x, cc2d.offset.y };

				b2FixtureDef fixture_def;
				fixture_def.shape = &circle_shape;

				fixture_def.density = cc2d.density;
				fixture_def.friction = cc2d.friction;
				fixture_def.restitution = cc2d.restitution;
				fixture_def.restitutionThreshold = cc2d.restitution_threshold;
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