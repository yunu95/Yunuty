#pragma once
#include "DamageInfo.h"
#include "YunutyEngine.h"

using namespace YunutyEngine;
class Destructible : public Component
{
public:
    virtual void ReceiveDamage(DamageInfo damageInfo) {}
    virtual void Kill()=0;
public:
    string name;
    double hp;
    double maxHp;
    double decayTime;
    bool canBeTargeted;
};

