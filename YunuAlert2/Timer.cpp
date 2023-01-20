#include "Timer.h"

void Timer::Update(double deltaTime)
{
    if (isActive)
    {
        elapsed += deltaTime;

        if (elapsed > duration)
            elapsed = duration;

        if (onUpdate)
            onUpdate(elapsed);
        if (onUpdateNormalized)
            onUpdateNormalized(elapsed / duration);

        if (elapsed == duration)
        {
            if (isRepeating)
                elapsed = 0;
            else
                isActive = false;
            if (onFinished)
                onFinished();
        }
    }
}
void Timer::Start()
{
    elapsed = 0;
    isActive = true;
}
