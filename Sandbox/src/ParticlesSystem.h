#pragma once

#include "glm/vec4.hpp"
#include "glm/vec3.hpp"
#include "Engine/Core/TimeStep.h"

struct ParticlesProp
{
	float life_time;
	glm::vec3 start_size;
	glm::vec3 end_size;
	glm::vec4 start_color;
	glm::vec4 end_color;
};

class ParticlesSystem
{
public:
	void Init(const ParticlesProp& prop);
	void Emit(const glm::vec3 pos);
	void OnUpdate(Engine::TimeStep ts);
private:
	struct Particle
	{
		glm::vec3 position;
		glm::vec3 size;
		glm::vec3 velocity;
		glm::vec4 color;

		float life_time_remained;
		bool is_active{ false };
	};
	const uint32_t k_max_particle_count = 100000;

	ParticlesProp m_prop;
	std::vector<Particle> m_particle_pool;
};