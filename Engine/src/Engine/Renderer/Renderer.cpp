#include "pch.h"

#include "Engine/Core/Core.h"
#include "Renderer.h"

#include "Renderer2D.h"
#include "RendererCommand.h"

namespace Engine
{

	void Renderer::Init()
	{
		PROFILER_FUNCTION();

		RendererCommand::Init();
		Renderer2D::Init();
	}

	void Renderer::OnWindowResized(uint32_t width, uint32_t height)
	{
		PROFILER_FUNCTION();

		RendererCommand::SetViewport(0, 0, width, height);
	}

	void Renderer::ShutDown()
	{
		PROFILER_FUNCTION();

		Renderer2D::ShutDown();
	}

} // namespace Engine