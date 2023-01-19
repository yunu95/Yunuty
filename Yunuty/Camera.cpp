#include "YunutyEngine.h"
#include "Camera.h"
using namespace YunutyEngine;
Camera* YunutyEngine::Camera::mainCamera = nullptr;
YunutyEngine::Camera::Camera()
{
    if (mainCamera == nullptr)
        mainCamera = this;
}
void YunutyEngine::Camera::SetCameraMain()
{
    mainCamera = this;
}
Camera* YunutyEngine::Camera::GetMainCamera()
{
    return mainCamera;
}
void YunutyEngine::Camera::SetNearPlane(const Rect& plane)
{
    this->nearPlane = plane;
}
Rect YunutyEngine::Camera::GetNearPlane()
{
    return nearPlane;
}
