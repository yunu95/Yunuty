#pragma once
#include <functional>
#include <vector>

using namespace std;
/// <summary>
/// ���� �ð��� ���� ������ �����ϰ� ������ ���Ǵ� ����̴�.
/// </summary>
class Timer
{
public:
    double elapsed=0;
    double duration=1;
    bool isActive;
    // Update�� update �Լ� �����͵��� elapsed, Ȥ�� ����ȭ�� elapsed�� �Ű������� ȣ���Ѵ�.
    void Update(double deltaTime);
    void Start();
    // �귯�� �ð��� �Ű������� ȣ���ϴ� �Լ� ������
    function<void(double)> onUpdate;
    // �귯�� �ð��� ����ȭ�Ͽ� �Ű������� ȣ���ϴ� �Լ� ������
    function<void(double)> onUpdateNormalized;
    function<void()> onFinished;
};
