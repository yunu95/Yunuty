#pragma once
#include <functional>
#include <vector>

using namespace std;
/// <summary>
/// 일정 시간에 걸쳐 뭔가를 진행하고 싶을때 사용되는 모듈이다.
/// </summary>
class Timer
{
public:
    double elapsed = 0;
    double duration = 1;
    bool isActive = false;
    bool isRepeating = false;
    // Update는 update 함수 포인터들을 elapsed, 혹은 정규화된 elapsed를 매개변수로 호출한다.
    void Update(double deltaTime);
    void Start();
    // 흘러간 시간을 매개변수로 호출하는 함수 포인터
    function<void(double)> onUpdate = [](double) {};

    // 흘러간 시간을 정규화하여 매개변수로 호출하는 함수 포인터
    function<void(double)> onUpdateNormalized = [](double) {};

    // 타이머가 끝났을 때 호출되는 함수 포인터
    function<void()> onFinished = []() {};
};

