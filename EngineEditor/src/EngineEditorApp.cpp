#include "Engine.h"
#include "Engine/Core/EntryPoint.h"

#include "EngineEditorLayer.h"

namespace Engine
{
	class EngineEditor : public Engine::Application
	{
	public:
		EngineEditor()
				:Engine::Application(1600, 900, "Engine Editor")
		{
			PushLayer(new EngineEditorLayer());
		}

		~EngineEditor() override = default;
	};
}
Engine::Application* CreateApplication()
{
	return new Engine::EngineEditor();
}