#pragma once
#include <glm/ext/matrix_transform.hpp>
#include "glm/glm.hpp"

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
			glm::mat4 rot = glm::rotate(glm::mat4(1.0f), rotation.x, { 1, 0, 0 })
					* glm::rotate(glm::mat4(1.0f), rotation.y, { 0, 1, 0 })
					* glm::rotate(glm::mat4(1.0f), rotation.z, { 0, 0, 1 });

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
		bool fix_aspectratio{ false };
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
}
