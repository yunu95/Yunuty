#pragma once
#include "Destructible.h"
#include "Unit.h"

class Projectile : public Destructible
{
public:
    bool isInstant = true;
    string projectileImage;
    //const Animation* deathAnimation;
    Unit* owner;
    Unit* target;
};
