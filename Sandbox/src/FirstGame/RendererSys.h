#pragma once

#include <vector>

#include "Engine.h"
#include "Quad.h"

class RendererSys
{
public:
	void RegisterQuad(Engine::Ref<Quad> quad);
	void RenderAll();
private:
	std::vector<Engine::Ref<Quad>> m_quads;
};


