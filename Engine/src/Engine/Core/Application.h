#pragma once

#include <Engine/ImGui/ImGuiLayer.h>
#include "Engine/Event/ApplicationEvent.h"
#include "LayerStack.h"
#include "Window.h"
#include "Core.h"

namespace Engine
{
	class Application
	{
	public:
		Application(uint32_t width = 1280, uint32_t height = 720, const std::string& title = "Engine");
		virtual ~Application() = default;
		void Run();
		void OnEvent(Event& event);

		void PushLayer(Layer* layer);
		void PushOverlay(Layer* layer);
		void EraseLayer(Layer* layer);

		inline static Application* get()
		{
			return s_instance;
		}

		inline std::shared_ptr<Window> GetWindow() const
		{
			return m_window;
		}
		inline ImGuiLayer* get_imgui_layer() const
		{
			return m_imgui_layer;
		}

	private:
		static Application* s_instance;

		bool OnWindowsClosed(WindowClosed& event);
		bool OnWindowsResized(WindowResized& event);
		void Close();

		Ref<Window> m_window;

		ImGuiLayer* m_imgui_layer;

		bool m_is_running{ true };
		bool m_is_minimized{ false };
		LayerStack m_layer_stack;

		float m_last_frame_time{ 0 };
	};
} // namespace Engine