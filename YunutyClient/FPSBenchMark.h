#pragma once
#include "YunutyEngine.h"
class FPSBenchMark : public Component
{
protected:
    virtual void Update()
    {
        wstringstream fpsStream;
        if (Time::GetDeltaTime())
            fpsStream <<  Time::GetFPS();
        if (GetGameObject()->GetComponent<D2DText>())
        {
            GetGameObject()->GetComponent<D2DText>()->text = fpsStream.str();
            GetGameObject()->GetComponent<D2DText>()->fontSize=40;
        }
    }
};

