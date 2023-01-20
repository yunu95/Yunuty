#include "YunutyEngine.h"
#include "DebugTile.h"
#include "DebugTilePool.h"

using namespace YunutyEngine;
DebugTile::DebugTile()
{
    glowingTimer.onUpdateNormalized = [this](double elapsed) {this->rectImage->color.a = elapsed < 0.5 ? 0 : 0.5 - 8 * (0.75 - elapsed) * (0.75 - elapsed); };
    glowingTimer.onFinished = [this]() {DebugTilePool::instance->Return(this); };
    glowingTimer.duration = 2;
}
void DebugTile::Update()
{
    glowingTimer.Update(Time::GetDeltaTime());
}
void DebugTile::StartGlowing()
{
    glowingTimer.Start();
}
void DebugTile::SetGlowingOffset(double progress)
{
    glowingTimer.elapsed = progress * glowingTimer.duration;
}
