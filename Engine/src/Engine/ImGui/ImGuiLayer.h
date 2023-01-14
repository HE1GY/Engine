#pragma once
#include "Engine/Layer.h"

#include "Engine/Event/Event.h"
#include "Engine/Event/ApplicationEvent.h"
#include "Engine/Event/KeyEvent.h"
#include "Engine/Event/MouseEvent.h"

namespace Engine
{
	class ImGuiLayer : public Layer
	{
	public:
		ImGuiLayer()
				:Layer("ImGuiLayer")
		{
		}
		virtual ~ImGuiLayer() override = default;

		virtual void OnAttach() override;

		virtual void OnDetach() override;
		virtual void OnUpdate(TimeStep ts) override;
		virtual void OnEvent(Event& event) override;

	private:
		bool OnWindowResized(WindowResized& e);
		bool OnWindowClosed(WindowClosed& e);
		bool OnKeyPress(KeyPress& e);
		bool OnKeyReleased(KeyReleased& e);
		bool OnKeyTyped(KeyTyped& e);
		bool OnMouseButtonPressed(MouseButtonPressed& e);
		bool OnMouseButtonReleased(MouseButtonReleased& e);
		bool OnMouseMoved(MouseMoved& e);
		bool OnMouseScrolled(MouseScrolled& e);

	};
}