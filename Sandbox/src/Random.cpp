#include "Random.h"

std::mt19937 Random::s_random_engine;
std::uniform_real_distribution<float> Random::s_distribution(0.0f, 1.0f);        //[0,1)
