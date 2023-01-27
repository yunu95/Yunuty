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
// �ش� Ű�ڵ�� ��ġ�ϴ� Ű�� ������ �ִٸ� ����, ������ ���� �ʴٸ� ������ ��ȯ�մϴ�.
// Ű�� ��� ������ �ִ� ���¶�� �� �����Ӹ��� ���� ��ȯ�մϴ�.
bool YunutyEngine::Input::isKeyDown(KeyCode keyCode)
{
    return instance->m_isKeyDown(keyCode);
}
// �ش� Ű�ڵ�� ��ġ�ϴ� Ű�� ������ �� �������ٸ� ����, �׷��� �ʴٸ� ������ ��ȯ�մϴ�.
// Ű�� ��� ������ �ִ� ���������� Ű�� ó�� ������ ������ �����ӿ����� ���� ��ȯ�մϴ�.
bool YunutyEngine::Input::isKeyPushed(KeyCode keyCode)
{
    return instance->m_isKeyPushed(keyCode);
}
// �ش� Ű�ڵ�� ��ġ�ϴ� Ű�� �� �������ٸ� ����, �׷��� �ʴٸ� ������ ��ȯ�մϴ�.
// Ű�� ��� ������ �ִ� ���������� Ű�� ó�� ������ ������ �����ӿ����� ���� ��ȯ�մϴ�.
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
