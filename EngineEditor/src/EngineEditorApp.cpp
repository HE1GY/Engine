#include "Engine.h"
#include "Engine/Core/EntryPoint.h"

#include "EngineEditorLayer.h"

namespace Engine
{
	class Engine_Editor : public Engine::Application
	{
	public:
		Engine_Editor()
		{
			PushLayer(new EngineEditorLayer());
		}

		~Engine_Editor() override = default;
	};
}
Engine::Application* CreateApplication()
{
	return new Engine::Engine_Editor();
}