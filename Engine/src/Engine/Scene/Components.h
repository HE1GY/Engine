#pragma once
#include <glm/ext/matrix_transform.hpp>
#include <glm/glm.hpp>

#define GLM_ENABLE_EXPERIMENTAL
#include <glm/gtx/quaternion.hpp>

#include "SceneCamera.h"
#include "ScriptableEntity.h"

namespace Engine
{

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
		glm::vec4 color{ 1, 1, 1, 1 };
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

	struct NativeScriptComponent
	{
		ScriptableEntity* instance{ nullptr };

		ScriptableEntity* (* InstantiateScript)();
		void (* DestroyScript)(NativeScriptComponent* nsc);

		template<typename T>
		void Bind()
		{
			InstantiateScript = []()
			{ return (ScriptableEntity*)new T(); };
			DestroyScript = [](NativeScriptComponent* nsc)
			{
			  delete nsc->instance;
			  nsc->instance = nullptr;
			};

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

}
