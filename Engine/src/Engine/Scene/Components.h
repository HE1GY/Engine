#pragma once
#include <glm/ext/matrix_transform.hpp>
#include <glm/glm.hpp>

#define GLM_ENABLE_EXPERIMENTAL
#include <glm/gtx/quaternion.hpp>
#include <Engine/Renderer/Texture.h>

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
		bool active = true;
		glm::vec4 color{ 1, 1, 1, 1 };
		Ref<Texture2D> texture;
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
		std::function<void(ScriptableEntity*)> AfterInstantiateScript;

		template<typename T>
		void Bind()
		{
			InstantiateScript = []()
			{
			  return (ScriptableEntity*)new T();
			};
		}

		void DestroyScript()
		{
			delete instance;
			instance = nullptr;
		}

	};

	//Physic
	struct Box2DComponent
	{
		glm::vec2 position;
		glm::vec2 size;
	};

	struct Circle2DComponent
	{
		glm::vec2 position;
		float radius;
	};

}
