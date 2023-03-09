#pragma once
#include "YunutyEngine.h"

class Timer
{
private:

public:
	double elapsed = 0;		// 타이머가 시작된 후 경과된 시간
	bool isActive = false;
	bool isRepeat = false;
	bool useScaledTime = true;
	double duration = 1;		// 목표로하는 시간 ex) 3초 타이머를 하고 싶다
	function<void()> onExpiration = []() {};								// elapsed >= duration
	function<void(double progress)> onUpdate = [](double progress) {};
	void Update();
	void Start();

};

