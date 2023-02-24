#pragma once
#include <glm/ext/matrix_transform.hpp>
#include <glm/glm.hpp>

#define GLM_ENABLE_EXPERIMENTAL
#include <glm/gtx/quaternion.hpp>
#include <Engine/Renderer/Texture.h>
#include <Engine/Core/UUID.h>

#include "SceneCamera.h"

namespace Engine
{
	struct IDComponent
	{
		UUID uuid;
	};

	struct TransformComponent
	{
		glm::vec3 translation{ 0.0f, 0.0f, 0.0f };
		glm::vec3 rotation{ 0.0f, 0.0f, 0.0f };
		glm::vec3 scale{ 1.0f, 1.0f, 1.0f };

		const glm::mat4 get_transformation() const
		{
			glm::mat4 rot = glm::toMat4(glm::quat(rotation));

			glm::mat4 transformation = glm::translate(glm::mat4(1.0f), translation)
					* rot
					* glm::scale(glm::mat4(1.0f), scale);

			return transformation;
		}

	};

	struct SpriteRendererComponent
	{
		bool active = true;
		glm::vec4 color{ 1, 1, 1, 1 };
		Ref<Texture2D> texture;
	};

	struct CircleRendererComponent
	{
		glm::vec4 color{ 1, 1, 1, 1 };
		float thickness = 1.0f;
		float fade = 0.005f;
	};

	struct LineRendererComponent
	{
		std::vector<glm::vec3> points;
		glm::vec4 color{ 1, 1, 1, 1 };
		float thickness = 1.0f;
	};

	struct TagComponent
	{
		TagComponent(const std::string& tag)
				:tag{ tag }
		{
		}
		std::string tag;
	};

	struct CameraComponent
	{
		CameraComponent() = default;

		bool primary{ false };
		bool fix_aspect_ratio{ false };
		SceneCamera camera;
	};

	//forward declaration
	class ScriptableEntity;
	struct NativeScriptComponent
	{
		Ref<ScriptableEntity> instance;
		Ref<ScriptableEntity> (* InstantiateScript)();

		std::function<void(Ref<ScriptableEntity>&)> AfterInstantiateScript;

		template<typename T>
		void Bind(std::function<void(Ref<ScriptableEntity>&)> after_instantiation_script = [](Ref<ScriptableEntity>&)
		{})
		{
			InstantiateScript = []()
			{
			  return static_pointer_cast<ScriptableEntity>(CreateRef<T>());
			};

			AfterInstantiateScript = after_instantiation_script;
		}
	};



	//Physics

	struct Rigidbody2DComponent
	{
		enum class BodyType
		{
			Static = 0, Dynamic, Kinematic
		};
		BodyType type = BodyType::Static;
		bool fixed_rotation = false;

		//storage for runtime
		void* runtime_body = nullptr;

		Rigidbody2DComponent() = default;
		Rigidbody2DComponent(const Rigidbody2DComponent& other) = default;
	};

	struct BoxCollider2DComponent
	{
		glm::vec2 offset = { 0.0f, 0.0f };
		glm::vec2 size = { 0.5f, 0.5f };

		float density = 1.0f;
		float friction = 0.5f;
		float restitution = 0.0f;
		float restitution_threshold = 0.5f;

		//storage for runtime
		void* RuntimeBody = nullptr;

		BoxCollider2DComponent() = default;
		BoxCollider2DComponent(const BoxCollider2DComponent& other) = default;
	};

	struct CircleCollider2DComponent
	{
		glm::vec2 offset = { 0.0f, 0.0f };
		float radius = 0.5f;

		float density = 1.0f;
		float friction = 0.5f;
		float restitution = 0.0f;
		float restitution_threshold = 0.5f;

		//storage for runtime
		void* RuntimeBody = nullptr;

		CircleCollider2DComponent() = default;
		CircleCollider2DComponent(const CircleCollider2DComponent& other) = default;

	};

}
