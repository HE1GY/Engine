#include "Application.h"

#include "Log.h"

namespace Engine
{

	Application::Application()
	{
		m_window = std::unique_ptr<Window>(Window::Create());

		m_window->SetEventCallback(std::bind(&Application::OnEvent, this, std::placeholders::_1));
	}
	void Application::Run()
	{
		while (m_is_running)
		{
			m_window->OnUpdate();
		}
	}
	void Application::OnEvent(Event& event)
	{
		CORE_INFO(event.ToString());
		EventDispatcher ed(event);
		std::function<bool(WindowClosed&)> on_window_closed = std::bind(&Application::OnWindowsClosed, this,
				std::placeholders::_1);
		ed.Dispatch<WindowClosed>(on_window_closed);
	}
	bool Application::OnWindowsClosed(WindowClosed& event)
	{
		m_is_running = false;
		return true;
	}
}