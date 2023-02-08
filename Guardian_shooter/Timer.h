#pragma once
#include "YunutyEngine.h"

class Timer
{
private:
	double elapsed = 0;		// Ÿ�̸Ӱ� ���۵� �� ����� �ð�

public:
	bool isActive = false;
	bool isRepeat = false;
	double duration = 1;		// ��ǥ���ϴ� �ð� ex) 3�� Ÿ�̸Ӹ� �ϰ� �ʹ�
	function<void()> onExpiration = []() {};								// elapsed >= duration
	function<void(double progress)> onUpdate = [](double progress) {};
	void Update();
	void Start();

};

