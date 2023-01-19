#include "YunutyEngine.h"
#include "YunutyTime.h"


double YunutyEngine::Time::timeScale = 1;
chrono::system_clock::time_point YunutyEngine::Time::lastFrameTime = (chrono::system_clock::time_point::min)();
double YunutyEngine::Time::deltaTimeUnscaled=0;
void YunutyEngine::Time::Update()
{
    auto now = chrono::system_clock::now();
    //std::chrono::duration<double> diff = now - lastFrameTime;
    if (lastFrameTime != (chrono::system_clock::time_point::min)())
        deltaTimeUnscaled = chrono::duration<double>(now - lastFrameTime).count();

    lastFrameTime = chrono::system_clock::now();
}
void YunutyEngine::Time::SetTimeScale(const float& timeScale)
{
    Time::timeScale = timeScale;
}
double YunutyEngine::Time::GetTimeScale()
{
    return timeScale;
}
double YunutyEngine::Time::GetDeltaTime()
{
    return deltaTimeUnscaled * timeScale;
}
double YunutyEngine::Time::GetDeltaTimeUnscaled()
{
    return deltaTimeUnscaled;
}
