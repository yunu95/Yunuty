#pragma once
#include "YunutyEngine.h"
#include "MapObject.h"

class ImageScroller : public Component
{
public:
    // 카메라가 x만큼 오른쪽으로 움직이면, 이미지는 변위 * (1-scrollRate) 만큼 오른쪽으로 움직인다.
    double scrollRate = 1.0;
    // 이미지의 위치 = scrollConstant + (1-scrollRate)
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