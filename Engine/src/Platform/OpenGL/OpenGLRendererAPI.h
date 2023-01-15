#pragma once

#include "Engine/Renderer/RendererAPI.h"

namespace Engine
{
	class OpenGLRendererAPI : public RendererAPI
	{
	public:
		virtual void Init() override;
		virtual void DrawIndex(std::shared_ptr<VertexArray> vertex_array) override;
		virtual void SetClearColor(const glm::vec4& color) override;
		virtual void Clear() override;
	};
}