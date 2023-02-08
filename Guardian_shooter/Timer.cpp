#include "Timer.h"

void Timer::Update()
{
	if (isActive)
	{
		elapsed += Time::GetDeltaTime();

		if (elapsed > duration)
			elapsed = duration;

		onUpdate(elapsed / duration);

		if (elapsed == duration)
		{
			onExpiration();
			if (isRepeat == true)
			{
				elapsed = 0;
			}
			else
			{
				isActive = false;
			}
		}


	}

}

void Timer::Start()		// 타이머를 0초부터 실행시키는 
{
	elapsed = 0;
	isActive = true;
}