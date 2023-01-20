#pragma once

#include "Transform.h"

class Quad
{
public:
	Quad(const glm::vec4& color, const Transform transform = {});

	glm::vec4& get_color()
	{
		return m_color;
	};
	Transform& get_transform()
	{
		return m_transform;
	};
private:
	Transform m_transform;
	glm::vec4 m_color;
};