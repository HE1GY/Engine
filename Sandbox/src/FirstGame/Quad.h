#pragma once
#include "glm/glm.hpp"

struct Quad
{
	glm::vec3 position;
	glm::vec2 scale;
	float rotation;
	glm::vec4 color;

	float get_left_bound() const
	{
		return position.x - (scale.x / 2);
	}
	float get_right_bound() const
	{
		return position.x + (scale.x / 2);
	}
	float get_bottom_bound() const
	{
		return position.y - (scale.y / 2);
	}
	float get_top_bound() const
	{
		return position.y + (scale.y / 2);
	}
};


