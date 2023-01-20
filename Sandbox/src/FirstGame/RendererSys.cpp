#include "pch.h"
#include "Engine/Renderer/Renderer2D.h"

#include "RendererSys.h"
void RendererSys::RenderAll()
{
	for (auto& quad : m_quads)
	{
		Engine::Renderer2D::DrawQuad(quad->get_transform().position, quad->get_transform().scale,
				quad->get_transform().rotation,
				quad->get_color());
	}
}
void RendererSys::RegisterQuad(Engine::Ref<Quad> quad)
{
	m_quads.push_back(quad);
}
