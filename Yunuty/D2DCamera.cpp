#include "YunutyEngine.h"

#include "D2DCamera.h"
#include "D2DGraphic.h"
#include <algorithm>
#include "GameObject.h"
#include "D2DCycle.h"


using namespace YunutyEngine;
using namespace YunutyEngine::D2D;

void D2DCamera::Render()
{
}
LRESULT CALLBACK D2DCamera::Render(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
    YunuD2D::YunuD2DGraphicCore::GetInstance()->BeginDraw();

    YunuD2D::YunuD2DGraphicCore::GetInstance()->ResizeResolution(resolutionW, resolutionH);
    SetNearPlane(Rect(resolutionW, resolutionH));

    auto halvedRenderSize = YunuD2D::YunuD2DGraphicCore::GetInstance()->GetRenderSize();
    halvedRenderSize.width /= 2;
    halvedRenderSize.height /= 2;

    vector<D2DGraphic*> graphics;

    // render world space graphics
    for (auto each : D2DGraphic::D2DGraphics[(int)CanvasRenderSpace::WorldSpace])
    {
        if (each->GetGameObject()->GetActive() && each->GetGameObject()->GetScene() == GetGameObject()->GetScene())
            graphics.push_back(each);
    }
    sort(graphics.begin(), graphics.end(), [](D2DGraphic*& item1, D2DGraphic*& item2)->bool
        {
            return item1->GetGameObject()->GetSceneIndex() < item2->GetGameObject()->GetSceneIndex();
        });

    for (auto each : graphics)
    {
        D2D1::Matrix3x2F eachTransform = D2D1::Matrix3x2F::Identity();
        auto camPos = GetTransform()->GetWorldPosition();
        auto pos = each->GetTransform()->GetWorldPosition();
        auto scale = each->GetTransform()->GetWorldScale();
        eachTransform = eachTransform * ScaleTransform(scale.x, scale.y);
        eachTransform = eachTransform * RotationTransform(each->GetTransform()->GetWorldRotation().Euler().z);
        eachTransform = eachTransform * TranslationTransform(halvedRenderSize.width + pos.x - camPos.x, halvedRenderSize.height - (pos.y - camPos.y));

        each->Render(eachTransform);
    }
    graphics.clear();

    // render camera space graphics
    for (auto each : D2DGraphic::D2DGraphics[(int)CanvasRenderSpace::CameraSpace])
    {
        if (each->GetGameObject()->GetActive() && each->GetGameObject()->GetScene() == GetGameObject()->GetScene())
            graphics.push_back(each);
    }
    sort(graphics.begin(), graphics.end(), [](D2DGraphic*& item1, D2DGraphic*& item2)->bool
        {
            return item1->GetGameObject()->GetSceneIndex() < item2->GetGameObject()->GetSceneIndex();
        });

    for (auto each : graphics)
    {
        D2D1::Matrix3x2F eachTransform = D2D1::Matrix3x2F::Identity();
        auto camPos = Vector2d::zero;
        auto pos = each->GetTransform()->GetWorldPosition();
        auto scale = each->GetTransform()->GetWorldScale();
        eachTransform = eachTransform * ScaleTransform(scale.x, scale.y);
        eachTransform = eachTransform * RotationTransform(each->GetTransform()->GetWorldRotation().Euler().z);
        eachTransform = eachTransform * TranslationTransform(halvedRenderSize.width + pos.x - camPos.x, halvedRenderSize.height - (pos.y - camPos.y));

        each->Render(eachTransform);
    }

    YunuD2D::YunuD2DGraphicCore::GetInstance()->EndDraw();
    return 0;
}

D2D1::Matrix3x2F D2DCamera::ScaleTransform(float x, float y)
{
    D2D1::Matrix3x2F ret = D2D1::Matrix3x2F::Identity();
    ret.m11 = x;
    ret.m22 = y;
    return ret;
}
D2D1::Matrix3x2F D2DCamera::RotationTransform(float angle)
{
    D2D1::Matrix3x2F ret = D2D1::Matrix3x2F::Identity();
    ret.m11 = cos(angle * YunutyMath::Deg2Rad);
    ret.m12 = -sin(angle * YunutyMath::Deg2Rad);
    ret.m21 = sin(angle * YunutyMath::Deg2Rad);
    ret.m22 = cos(angle * YunutyMath::Deg2Rad);
    return ret;
}
D2D1::Matrix3x2F D2DCamera::TranslationTransform(float dx, float dy)
{
    D2D1::Matrix3x2F ret = D2D1::Matrix3x2F::Identity();
    ret.dx = dx;
    ret.dy = dy;
    return ret;
}
Rect D2DCamera::GetScaledResolution()
{
    return Rect(resolutionW * resolutionFactor, resolutionH * resolutionFactor);
}
D2DCamera* D2DCamera::GetMainD2DCamera()
{
    return dynamic_cast<D2DCamera*>(GetMainCamera());
}
