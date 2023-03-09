#pragma once
#include "YunutyEngine.h"
#include "MapObject.h"

class ImageScroller : public Component
{
public:
    // ī�޶� x��ŭ ���������� �����̸�, �̹����� ���� * (1-scrollRate) ��ŭ ���������� �����δ�.
    double scrollRate = 1.0;
    // �̹����� ��ġ = scrollConstant + (1-scrollRate)
    double scrollConstant = 0;
    virtual void Update() override
    {
        if (scrollRate != 1)
        {
            ApplyScrollLogic();
        }
    };
private:
    void ApplyScrollLogic();
};