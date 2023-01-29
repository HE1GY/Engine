#pragma once
#include "glm/glm.hpp"
#include "SceneCamera.h"
#include "ScriptableEntity.h"

namespace Engine
{

	struct TransformComponent
	{
		glm::mat4 transform{ 1.0f };

		operator glm::mat4() const
		{
			return transform;
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
