#include "YunutyEngine.h"
#include "YunutyTime.h"


double YunutyEngine::Time::timeScale = 1;
chrono::system_clock::time_point YunutyEngine::Time::lastFrameTime = (chrono::system_clock::time_point::min)();
double YunutyEngine::Time::deltaTimeUnscaled = 0;
queue<double> YunutyEngine::Time::fpsQueue;
void YunutyEngine::Time::Update()
{
    auto now = chrono::system_clock::now();
    //std::chrono::duration<double> diff = now - lastFrameTime;
    if (lastFrameTime != (chrono::system_clock::time_point::min)())
    {
        deltaTimeUnscaled = chrono::duration<double>(now - lastFrameTime).count();
#if _DEBUG
        if (deltaTimeUnscaled > 1)
            deltaTimeUnscaled = 0.000001;
#endif
    }

    lastFrameTime = now;
    auto nowInSeconds = ((chrono::duration<double>)now.time_since_epoch()).count();
    fpsQueue.push(nowInSeconds);
    while (!fpsQueue.empty() && fpsQueue.front() < nowInSeconds - 1)
    {
        fpsQueue.pop();
    }
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
int YunutyEngine::Time::GetFPS()
{
    return (int)fpsQueue.size();
}
