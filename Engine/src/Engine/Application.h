#pragma once

#include <memory>

#include "Window.h"
#include "Engine/Event/ApplicationEvent.h"

namespace Engine
{
	class Application
	{
	public:
		Application();
		void Run();
		void OnEvent(Event& event);

	private:
		bool OnWindowsClosed(WindowClosed& event);

		std::unique_ptr<Window> m_window;
		bool m_is_running{ true };
	};
}