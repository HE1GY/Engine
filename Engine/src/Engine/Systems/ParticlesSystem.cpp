
#include "ParticlesSystem.h"
#include "Engine/Renderer/Renderer2D.h"

#include "Random.h"

namespace Engine
{
	template<typename T>
	T Lerp(const T& a, const T& b, float f)
	{
		return T((a * (1.0f - f)) + (b * f));
	}

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
				particle.size = m_prop.start_size;

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

				particle.size = Lerp(m_prop.start_size, m_prop.end_size, life);

				if (particle.life_time_remained <= 0)
				{
					particle.is_active = false;
				}
			}
		}
	}
	void ParticlesSystem::OnRenderer(Engine::OrthographicCamera& cam)
	{
		Engine::Renderer2D::BeginScene(cam.get_projection_matrix(), cam.get_view_matrix());
		for (auto& particle : m_particle_pool)
		{
			if (particle.is_active)
			{
				//Engine::Renderer2D::DrawQuad(particle.position, { particle.size.x, particle.size.y }, particle.color);//TODO::redo
			}
		}

		Engine::Renderer2D::EndScene();
	}

}