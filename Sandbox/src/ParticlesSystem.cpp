
#include "ParticlesSystem.h"
#include "Engine/Renderer/Renderer2D.h"

#include "Random.h"

glm::vec4 Lerp(const glm::vec4& a, const glm::vec4& b, float f);

void ParticlesSystem::Init(const ParticlesProp& prop)
{
	m_prop = prop;
	m_particle_pool.resize(k_max_particle_count);

	Random::Init();
}
void ParticlesSystem::Emit(const glm::vec3 pos)
{
	for (auto& particle : m_particle_pool)
	{
		if (!particle.is_active)
		{
			particle.is_active = true;
			particle.position = pos;
			particle.size = m_prop.size;

			float x = Random::Float() * 2 - 1;
			float y = Random::Float() * 2 - 1;

			particle.velocity = { x, y, 0 };

			particle.color = m_prop.start_color;
			particle.life_time_remained = m_prop.life_time;
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

			float life = (particle.life_time_remained / m_prop.life_time - (m_prop.life_time / 2)) * -1;
			particle.color = Lerp(m_prop.start_color, m_prop.end_color, life);

			//color
			if (particle.life_time_remained <= 0)
			{
				particle.is_active = false;
			}
		}
	}
}
void ParticlesSystem::OnRenderer(Engine::OrthographicCamera& cam)
{
	Engine::Renderer2D::BeginScene(cam);
	for (auto& particle : m_particle_pool)
	{
		if (particle.is_active)
		{
			Engine::Renderer2D::DrawQuad(particle.position, { particle.size.x, particle.size.y }, particle.color);
		}
	}

	Engine::Renderer2D::EndScene();
}

glm::vec4 Lerp(const glm::vec4& a, const glm::vec4& b, float f)
{
	return glm::vec4((a * (1.0f - f)) + (b * f));
}
