#pragma once
#include "YunutyEngine.h"
#include "Timer.h"

using namespace std;
using namespace YunutyEngine;

class DebugTile :public Component
{
private:
    Timer glowingTimer;
protected:
    void Update()override;
public:
    DebugTile();
    void StartGlowing();
    void SetGlowingOffset(double progress);
    D2DRectangle* rectImage = nullptr;
};
