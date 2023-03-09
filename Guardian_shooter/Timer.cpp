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

void Timer::Start()		// Ÿ�̸Ӹ� 0�ʺ��� �����Ű�� 
{
    elapsed = 0;
    isActive = true;
}