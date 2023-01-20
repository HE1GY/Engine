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
		PROFILER_FUNCTION();

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
		PROFILER_FUNCTION();

		while (m_is_running)
		{
			float time = (float)glfwGetTime(); // TODO glfw dep
			TimeStep ts = time - m_last_frame_time;
			m_last_frame_time = time;

			if (!m_is_minimized)
			{
				{
					PROFILER_SCOPE("Layers OnUpdate");

					for (auto layer : m_layer_stack)
					{
						layer->OnUpdate(ts);
					}
				}

				{
					PROFILER_SCOPE("Layers ImGuiRenderer");

					m_imGuiLayer->Begin();
					for (auto layer : m_layer_stack)
					{
						layer->OnImGuiRender();
					}
					m_imGuiLayer->End();
				}
			}

			m_window->OnUpdate();
		}
	}
	void Application::OnEvent(Event& event)
	{
		PROFILER_FUNCTION();

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
		PROFILER_FUNCTION();
		m_is_running = false;
		ShutDown();
		return true;
	}
	bool Application::OnWindowsResized(WindowResized& event)
	{
		PROFILER_FUNCTION();
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
		PROFILER_FUNCTION();
		m_layer_stack.PushLayer(layer);
		layer->OnAttach();
	}
	void Application::PushOverlay(Layer* layer)
	{
		PROFILER_FUNCTION();
		m_layer_stack.PushOverlay(layer);
		layer->OnAttach();
	}
	void Application::EraseLayer(Layer* layer)
	{
		PROFILER_FUNCTION();
		m_layer_stack.EraseLayer(layer);
		layer->OnDetach();
	}
	void Application::ShutDown()
	{
		PROFILER_FUNCTION();
		Renderer::ShutDown();
	}

} // namespace Engine