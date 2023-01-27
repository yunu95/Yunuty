#include "ZoomableCamera.h"
void ZoomableCamera::Update()
{
    constexpr double zoomSpeed = 1;
    if (D2DInput::isKeyDown(KeyCode::S))
        SetZoomOutFactor(GetZoomOutFactor() + zoomSpeed * Time::GetDeltaTime());
    if (D2DInput::isKeyDown(KeyCode::W))
        SetZoomOutFactor(GetZoomOutFactor() - zoomSpeed * Time::GetDeltaTime());
}
