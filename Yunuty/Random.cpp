#include "Random.h"

using namespace YunutyEngine;
using namespace YunutyEngine::YunutyMath;

Random* Random::instance = nullptr;
Random::Random()
{
    gen = mt19937(rd());
}
int Random::GetRandomInt(int min, int max)
{
    std::uniform_int_distribution<int> dis(min, max);
    return dis(GetInstance()->gen);
}
Random* Random::GetInstance()
{
    if (instance == nullptr)
        instance = new Random();
    return instance;
}
