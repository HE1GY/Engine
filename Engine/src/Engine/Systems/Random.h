#pragma once

#include <random>

namespace Engine
{
	class Random
	{
	public:
		static void Init()
		{
			s_random_engine.seed(std::random_device()());
		}

		static float Float()
		{
			return s_distribution(s_random_engine);
		}

	private:
		static std::mt19937 s_random_engine;
		static std::uniform_real_distribution<float> s_distribution;
	};
}


