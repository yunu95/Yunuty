#include "Timer.h"

void Timer::Update()
{
    if (isActive)
    {
        elapsed += useScaledTime ? Time::GetDeltaTime() : Time::GetDeltaTimeUnscaled();

        if (elapsed > duration)
            elapsed = duration;

        if (onUpdate)
            onUpdate(elapsed / duration);

        if (elapsed == duration)
        {
            if (isRepeat == true)
            {
                elapsed = 0;
            }
            else
            {
                isActive = false;
            }
 //           if (onExpiration)
            onExpiration();
        }
    }
}

void Timer::Start()		// 타이머를 0초부터 실행시키는 
{
    elapsed = 0;
    isActive = true;
}