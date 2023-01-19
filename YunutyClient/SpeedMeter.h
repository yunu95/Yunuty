#pragma once
#include "Component.h"
#include "YunutyEngine.h"
class SpeedMeter :
    public Component
{
public:
    RigidBody2D* rigidBody = nullptr;
    virtual void Update() override;
};

