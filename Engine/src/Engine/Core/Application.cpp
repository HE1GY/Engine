#include "pch.h"

#include "Application.h"
#include "Core.h"

#include "Engine/Core/TimeStep.h"
#include "Engine/Renderer/Renderer.h"

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
		m_window->SetEventCallback(BIND_EVENT_FUNC(Application::OnEvent));

		Renderer::Init();

		m_imGuiLayer = new ImGuiLayer();
		PushOverlay(m_imGuiLayer);
	}

	void Application::Run()
	{
		while (m_is_running)
		{
			float time = (float)glfwGetTime(); // TODO glfw dep
			TimeStep ts = time - m_last_frame_time;
			m_last_frame_time = time;

			if (!m_is_minimized)
			{
				for (auto layer : m_layer_stack)
				{
					layer->OnUpdate(ts);
				}
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

		ed.Dispatch<WindowClosed>(BIND_EVENT_FUNC(Application::OnWindowsClosed));
		ed.Dispatch<WindowResized>(BIND_EVENT_FUNC(Application::OnWindowsResized));

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
		ShutDown();
		return true;
	}
	bool Application::OnWindowsResized(WindowResized& event)
	{
		if (event.get_height() == 0 || event.get_width() == 0)
		{
			m_is_minimized = true;
			return false;
		}
		m_is_minimized = false;

		Renderer::OnWindowResized(event.get_width(), event.get_height());
		return false;
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
	void Application::ShutDown()
	{
		Renderer::ShutDown();
	}

} // namespace Engine