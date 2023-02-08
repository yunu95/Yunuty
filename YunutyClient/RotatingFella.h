#pragma once
#include "YunutyEngine.h"

class RotatingFella :public YunutyEngine::Component
{
protected:
    void Update() override
    {
        auto eulerRot = GetTransform()->GetWorldRotation().Euler();
        eulerRot.z += 10 * Time::GetDeltaTime();
        GetTransform()->SetWorldRotation(eulerRot);
    }
};

