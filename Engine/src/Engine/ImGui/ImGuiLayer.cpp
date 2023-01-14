#include <Engine/Event/KeyEvent.h>
#include "pch.h"

#include "ImGuiLayer.h"

#include "imgui_impl_opengl3.h"
#include "imgui_impl_glfw.h"

#include "Engine/Application.h"
#include "Engine/KeyCode.h"

namespace Engine
{
	void ImGuiLayer::OnAttach()
	{
		ImGui::CreateContext();
		ImGui::StyleColorsDark();

		ImGuiIO& io = ImGui::GetIO();
		io.BackendFlags |= ImGuiBackendFlags_HasMouseCursors;
		io.BackendFlags |= ImGuiBackendFlags_HasSetMousePos;

		ImGui_ImplOpenGL3_Init("#version 460");

	}

	void ImGuiLayer::OnDetach()
	{
	}
	void ImGuiLayer::OnUpdate(TimeStep ts)
	{

		ImGuiIO& io = ImGui::GetIO();
		io.DeltaTime = ts;
		Application* app = Application::Get();
		io.DisplaySize = ImVec2(app->GetWindow()->get_width(), app->GetWindow()->get_height());

		ImGui_ImplOpenGL3_NewFrame();
		ImGui::NewFrame();

		static bool show = true;
		ImGui::ShowDemoWindow(&show);

		ImGui::Render();
		ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
	}
	void ImGuiLayer::OnEvent(Event& event)
	{

		EventDispatcher ed(event);

		ed.Dispatch<WindowResized>(BIND_EVENT_FUNC(ImGuiLayer::OnWindowResized));
		ed.Dispatch<WindowClosed>(BIND_EVENT_FUNC(ImGuiLayer::OnWindowClosed));
		ed.Dispatch<KeyPress>(BIND_EVENT_FUNC(ImGuiLayer::OnKeyPress));
		ed.Dispatch<KeyReleased>(BIND_EVENT_FUNC(ImGuiLayer::OnKeyReleased));
		ed.Dispatch<KeyTyped>(BIND_EVENT_FUNC(ImGuiLayer::OnKeyTyped));
		ed.Dispatch<MouseButtonPressed>(BIND_EVENT_FUNC(ImGuiLayer::OnMouseButtonPressed));
		ed.Dispatch<MouseButtonReleased>(BIND_EVENT_FUNC(ImGuiLayer::OnMouseButtonReleased));
		ed.Dispatch<MouseMoved>(BIND_EVENT_FUNC(ImGuiLayer::OnMouseMoved));
		ed.Dispatch<MouseScrolled>(BIND_EVENT_FUNC(ImGuiLayer::OnMouseScrolled));

	}

	bool ImGuiLayer::OnWindowResized(WindowResized& e)
	{
		ImGuiIO& io = ImGui::GetIO();
		io.DisplaySize = ImVec2(e.get_height(), e.get_width());
		return false;
	}
	bool ImGuiLayer::OnWindowClosed(WindowClosed& e)
	{
		ImGuiIO& io = ImGui::GetIO();

		return false;
	}
	bool ImGuiLayer::OnKeyPress(KeyPress& e)
	{
		ImGuiIO& io = ImGui::GetIO();
		io.AddKeyEvent(ImGui_ImplGlfw_KeyToImGuiKey(e.get_key()), true);
		return false;
	}
	bool ImGuiLayer::OnKeyReleased(KeyReleased& e)
	{
		ImGuiIO& io = ImGui::GetIO();
		io.AddKeyEvent(ImGui_ImplGlfw_KeyToImGuiKey(e.get_key()), false);
		return false;
	}
	bool ImGuiLayer::OnKeyTyped(KeyTyped& e)
	{
		ImGuiIO& io = ImGui::GetIO();
		io.AddInputCharacter((unsigned int)e.get_key());
		return false;
	}
	bool ImGuiLayer::OnMouseButtonPressed(MouseButtonPressed& e)
	{
		ImGuiIO& io = ImGui::GetIO();
		io.AddMouseButtonEvent((unsigned int)e.get_key(), true);
		return false;
	}
	bool ImGuiLayer::OnMouseButtonReleased(MouseButtonReleased& e)
	{
		ImGuiIO& io = ImGui::GetIO();
		io.AddMouseButtonEvent((unsigned int)e.get_key(), false);
		return false;
	}
	bool ImGuiLayer::OnMouseMoved(MouseMoved& e)
	{
		ImGuiIO& io = ImGui::GetIO();
		io.AddMousePosEvent(e.get_x(), e.get_y());
		return false;
	}
	bool ImGuiLayer::OnMouseScrolled(MouseScrolled& e)
	{
		ImGuiIO& io = ImGui::GetIO();
		io.AddMouseWheelEvent(e.get_x_offset(), e.get_y_offset());
		return false;
	}
}
