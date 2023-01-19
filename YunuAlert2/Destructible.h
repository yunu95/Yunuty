#pragma once
#include "YunutyEngine.h"

using namespace YunutyEngine;
class Destructible : public Component
{
public:
    void Kill();
public:
    string name;
    double hp;
    double maxHp;
    double decayTime;
    bool canBeTargeted;
};

