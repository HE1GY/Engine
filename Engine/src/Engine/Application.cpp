#include "pch.h"

#include "Application.h"
#include "Core.h"

#include "Core/TimeStep.h"
#include "GLFW/glfw3.h"

namespace Engine
{

	Application* Application::s_instance = nullptr;

	Application::Application()
	{
		CORE_ASSERT((s_instance == nullptr), "App exists");
		s_instance = this;

		m_window = Ref<Window>(Window::Create());
		m_window->SetVSync(true);
		m_window->SetEventCallback(std::bind(&Application::OnEvent, this, std::placeholders::_1));

		m_imGuiLayer = new ImGuiLayer();
		PushOverlay(m_imGuiLayer);
	}

	void Application::Run()
	{
		while (m_is_running)
		{
			float time = (float)glfwGetTime(); //TODO glfw dep
			TimeStep ts = time - m_last_frame_time;
			m_last_frame_time = time;

			for (auto layer : m_layer_stack)
			{
				layer->OnUpdate(ts);
			}

			m_imGuiLayer->Begin();
			for (auto layer : m_layer_stack)
			{
				layer->OnImGuiRender();
			}
			m_imGuiLayer->End();

			m_window->OnUpdate();
		}
	}
	void Application::OnEvent(Event& event)
	{
		EventDispatcher ed(event);
		std::function<bool(WindowClosed&)> on_window_closed =
				std::bind(&Application::OnWindowsClosed, this, std::placeholders::_1);
		ed.Dispatch<WindowClosed>(on_window_closed);

		for (auto it = m_layer_stack.end(); it != m_layer_stack.begin();)
		{
			(*--it)->OnEvent(event);
			if (event.Handled)
			{
				break;
			}
		}
	}
	bool Application::OnWindowsClosed(WindowClosed& event)
	{
		m_is_running = false;
		return true;
	}
	void Application::PushLayer(Layer* layer)
	{
		m_layer_stack.PushLayer(layer);
	}
	void Application::PushOverlay(Layer* layer)
	{
		m_layer_stack.PushOverlay(layer);
	}
	void Application::EraseLayer(Layer* layer)
	{
		m_layer_stack.EraseLayer(layer);
	}

} // namespace Engine