#pragma once
#include "Engine/Core/Layer.h"

#include "Engine/Event/ApplicationEvent.h"
#include "Engine/Event/Event.h"
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
		virtual void OnEvent(Event& event) override;

		void Begin();
		void End();

		inline void set_block_event(bool on)
		{
			m_block_event = on;
		};
	
		void SetDarkThemeColors();

	private:
		bool m_block_event{ false };
	};
} // namespace Engine