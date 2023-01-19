#pragma once
#include "YunutyEngine.h"

class RTSCamera : public D2DCamera
{
protected:
    double scrollSpeed = 800;
    virtual void Update() override;
};

