#include "RTSCamera.h"

void RTSCamera::Update()
{
    Vector2d inputDirection;
    if (D2DInput::getMousePositionNormalized().x >= 1)
        inputDirection.x += 1;
    if (D2DInput::getMousePositionNormalized().x <= 0)
        inputDirection.x -= 1;
    if (D2DInput::getMousePositionNormalized().y >= 1)
        inputDirection.y -= 1;
    if (D2DInput::getMousePositionNormalized().y <= 0)
        inputDirection.y += 1;

    GetTransform()->position += inputDirection.Normalized() * Time::GetDeltaTimeUnscaled() * scrollSpeed;
}
