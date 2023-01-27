#include "YunutyEngine.h"
#include "Input.h"

using namespace YunutyEngine;

unique_ptr<Input> YunutyEngine::Input::instance = nullptr;

YunutyEngine::Input::Input()
{
    if (YunutyEngine::Input::instance.get() == nullptr)
        instance = unique_ptr<Input>(this);
    //DirectInput8Create(GetModuleHandle(NULL), DIRECTINPUT_VERSION, IID_IDirectInput8, (void**)&directInput, NULL);
}
// 해당 키코드와 일치하는 키가 눌러져 있다면 참을, 눌러져 있지 않다면 거짓을 반환합니다.
// 키가 계속 눌려져 있는 상태라면 매 프레임마다 참을 반환합니다.
bool YunutyEngine::Input::isKeyDown(KeyCode keyCode)
{
    return instance->m_isKeyDown(keyCode);
}
// 해당 키코드와 일치하는 키가 새로이 갓 눌러졌다면 참을, 그렇지 않다면 거짓을 반환합니다.
// 키가 계속 눌려져 있는 상태일지라도 키가 처음 눌러진 순간의 프레임에서만 참을 반환합니다.
bool YunutyEngine::Input::isKeyPushed(KeyCode keyCode)
{
    return instance->m_isKeyPushed(keyCode);
}
// 해당 키코드와 일치하는 키가 갓 떼어졌다면 참을, 그렇지 않다면 거짓을 반환합니다.
// 키가 계속 떼어져 있는 상태일지라도 키가 처음 떼어진 순간의 프레임에서만 참을 반환합니다.
bool YunutyEngine::Input::isKeyLifted(KeyCode keyCode)
{
    return instance->m_isKeyLifted(keyCode);
}
Vector3d YunutyEngine::Input::getMouseWorldPosition()
{
    auto mousePos = getMousePositionNormalized();
    mousePos -= Vector2d::one * 0.5;
    mousePos.y *= -1;
    mousePos.x *= Camera::GetMainCamera()->GetNearPlane().width;
    mousePos.y *= Camera::GetMainCamera()->GetNearPlane().height;
    return Camera::GetMainCamera()->GetTransform()->GetWorldPosition() + mousePos;
}
Vector2d YunutyEngine::Input::getMousePosition()
{
    POINT point;
    GetCursorPos(&point);
    ScreenToClient(D2DCycle::GetInstance().GetMainWindow(), &point);
    return Vector2d(point.x, point.y);
}
Vector2d YunutyEngine::Input::getMousePositionNormalized()
{
    RECT clientRect;
    GetClientRect(D2DCycle::GetInstance().GetMainWindow(), &clientRect);
    int clientWidth = clientRect.right - clientRect.left;
    int clientHeight = clientRect.bottom - clientRect.top;
    auto pos = getMousePosition();
    pos.x /= clientWidth;
    pos.y /= clientHeight;
    return pos;
}
