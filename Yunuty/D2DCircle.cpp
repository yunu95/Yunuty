#include "YunutyEngine.h"
#include "D2DCircle.h"
#include "Transform.h"
#include "YunuD2DGraphicCore.h"

using namespace YunutyEngine;
using namespace YunutyEngine::D2D;

void D2DCircle::Render(D2D1::Matrix3x2F transform)
{
    YunuD2D::YunuD2DGraphicCore::GetInstance()->DrawCircle(transform, radius, border, color, filled);

    YunuD2D::YunuD2DGraphicCore::GetInstance()->DrawSprite(L"1.png", transform, D2D1::ColorF::White, 200, 200);
}
