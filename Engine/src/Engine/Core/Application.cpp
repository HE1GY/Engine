#include "pch.h"

#include "Application.h"
#include "Core.h"

#include "Engine/Core/TimeStep.h"
#include "Engine/Renderer/Renderer.h"

namespace Engine
{

	Application* Application::s_instance = nullptr;

	Application::Application(uint32_t width, uint32_t height, const std::string& title)
	{
		PROFILER_FUNCTION();

		CORE_ASSERT((s_instance == nullptr), "Applicaion already exists");
		s_instance = this;

		m_window = Ref<Window>(Window::Create({ width, height, title }));
		m_window->set_VSync(true);
		m_window->set_event_callback(BIND_EVENT_FUNC(Application::OnEvent));

		Renderer::Init();

		m_imgui_layer = new ImGuiLayer();
		PushOverlay(m_imgui_layer);
	}

	void Application::Close()
	{
		PROFILER_FUNCTION();
		m_is_running = false;
		Renderer::ShutDown();
	}

	void Application::Run()
	{
		PROFILER_FUNCTION();

		while (m_is_running)
		{
			float time = m_window->get_current_time();
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

					m_imgui_layer->Begin();
					for (auto layer : m_layer_stack)
					{
						layer->OnImGuiRender();
					}
					m_imgui_layer->End();
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
		Close();
		return true;
	}
	bool Application::OnWindowsResized(WindowResized& event)
	{
		PROFILER_FUNCTION();
		if (event.GetHeight() == 0 || event.GetWidth() == 0)
		{
			m_is_minimized = true;
			return false;
		}
		m_is_minimized = false;

		Renderer::OnWindowResized(event.GetWidth(), event.GetHeight());
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

} // namespace Engine