#include "Guardian_shooter.h"
#include "ImageScroller.h"

void ImageScroller::ApplyScrollLogic()
{
    auto pos = GetTransform()->position;
    pos.x = scrollConstant + GSCamera::GetInstance()->GetTransform()->position.x * (1 - scrollRate);
    GetTransform()->position = pos;
}
