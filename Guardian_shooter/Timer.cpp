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

void Timer::Start()		// Ÿ�̸Ӹ� 0�ʺ��� �����Ű�� 
{
	elapsed = 0;
	isActive = true;
}