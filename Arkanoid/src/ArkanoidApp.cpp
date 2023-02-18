#pragma once
#include <Engine/Core/EntryPoint.h>
#include "ArkanoidLayer.h"

namespace Arkanoid
{
	class ArkanoidApp : public Engine::Application
	{
	public:
		ArkanoidApp()
				:Engine::Application(1280, 720, "Arkanoid")
		{
			m_layer = new ArkanoidLayer();
			PushLayer(m_layer);
		}

		virtual ~ArkanoidApp() override
		{
			EraseLayer(m_layer);
			delete m_layer;
		}
	private:
		Engine::Layer* m_layer;
	};

}
Engine::Application* CreateApplication()
{
	return new Arkanoid::ArkanoidApp();
}