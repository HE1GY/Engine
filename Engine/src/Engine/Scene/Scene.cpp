#include "pch.h"

#include "Scene.h"
#include "Entity.h"
#include "Components.h"
#include "Engine/Renderer/Renderer2D.h"
#include "Engine/Renderer/Camera.h"

namespace Engine
{

	Entity Scene::CreateEntity(const std::string& tag)
	{
		auto entity = Entity(m_registry.create(), this);
		entity.AddComponent<TransformComponent>();
		entity.AddComponent<TagComponent>(tag);
		return entity;
	}
	void Scene::OnUpdate(TimeStep ts)
	{
		Camera* main_camera{ nullptr };
		glm::mat4* cam_transform{ nullptr };

		auto group = m_registry.view<TransformComponent, CameraComponent>();
		for (auto entity : group)
		{
			auto [transform, cameraComponent] = group.get<TransformComponent, CameraComponent>(entity);

			if (cameraComponent.primary)
			{
				main_camera = &cameraComponent.camera;
				cam_transform = &transform.transform;
				break;
			}
		}

		if (main_camera)
		{
			Renderer2D::BeginScene(*main_camera, *cam_transform);

			auto group = m_registry.group<TransformComponent>(entt::get<SpriteRendererComponent>);

			for (auto entity : group)
			{
				auto [transform, sprite_renderer] = m_registry.get<TransformComponent, SpriteRendererComponent>(entity);
				Renderer2D::DrawQuad(transform, sprite_renderer.color);
			}

			Renderer2D::EndScene();
		}

	}
}