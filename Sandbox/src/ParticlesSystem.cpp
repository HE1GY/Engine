
#include "ParticlesSystem.h"
#include "Engine/Renderer/Renderer2D.h"

void ParticlesSystem::Init(const ParticlesProp& prop)
{
	m_prop = prop;
	m_particle_pool.resize(k_max_particle_count);
}
void ParticlesSystem::Emit(const glm::vec3 pos)
{
	for (auto& particle : m_particle_pool)
	{
		if (!particle.is_active)
		{
			particle.is_active = true;
			particle.position = pos;
			particle.velocity={,,0};

			particle.color=m_prop.start_color;
			particle.life_time_remained=m_prop.life_time;
			break;
		}

	}
}
void ParticlesSystem::OnUpdate(Engine::TimeStep ts)
{
	for (auto& particle : m_particle_pool)
	{
		if (particle.is_active)
		{
			particle.life_time_remained -= ts;
			particle.position += particle.velocity * ts.GetInSeconds();

			//color
			if (particle.life_time_remained <= 0)
			{
				particle.is_active = false;
			}
		}
	}
}
void ParticlesSystem::OnRenderer()
{
	for (auto& particle : m_particle_pool)
	{
		if (particle.is_active)
		{
			Engine::Renderer2D::DrawQuad(particle.position, { 1, 1 }, particle.color);
		}
	}
}
