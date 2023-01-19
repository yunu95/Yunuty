#include "YunutyEngine.h"
#include "D2DInput.h"

using namespace YunutyEngine;
using namespace YunutyEngine::D2D;

D2DInput* D2DInput::instance = nullptr;

D2DInput::D2DInput()
{
    instance = this;
    rawKeyEnumMap.insert(make_pair(VK_LEFT, KeyCode::LeftArrow));
    rawKeyEnumMap.insert(make_pair(VK_RIGHT, KeyCode::RightArrow));
    rawKeyEnumMap.insert(make_pair(VK_UP, KeyCode::UpArrow));
    rawKeyEnumMap.insert(make_pair(VK_DOWN, KeyCode::DownArrow));
    rawKeyEnumMap.insert(make_pair(VK_SPACE, KeyCode::Space));
    rawKeyEnumMap.insert(make_pair(VK_RETURN, KeyCode::Return));
    rawKeyEnumMap.insert(make_pair(VK_LBUTTON, KeyCode::MouseLeftClick));
    rawKeyEnumMap.insert(make_pair(VK_MBUTTON, KeyCode::MouseMiddleClick));
    rawKeyEnumMap.insert(make_pair(VK_RBUTTON, KeyCode::MouseRightClick));

    rawKeyEnumMap.insert(make_pair('A', KeyCode::A));
    rawKeyEnumMap.insert(make_pair('B', KeyCode::B));
    rawKeyEnumMap.insert(make_pair('a', KeyCode::a));
    rawKeyEnumMap.insert(make_pair('b', KeyCode::b));
}
D2DInput::~D2DInput()
{

}
void D2DInput::Update()
{
    keyPushedSet.clear();
    keyLiftedSet.clear();
    for (auto each : rawKeyEnumMap)
    {
        if (GetKeyState(each.first) & 0x8000)
        {
            if (keyDownSet.find(each.second) == keyDownSet.end())
                keyPushedSet.insert(each.second);
            keyDownSet.insert(each.second);
        }
        else
        {
            if (keyDownSet.find(each.second) != keyDownSet.end())
                keyLiftedSet.insert(each.second);
            keyDownSet.erase(each.second);
        }
    }
}
LRESULT CALLBACK D2DInput::WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
    /*keyPushedSet.clear();
    switch (message)
    {
    case WM_LBUTTONDOWN:
    case WM_MBUTTONDOWN:
    case WM_RBUTTONDOWN:
    case WM_KEYDOWN:
        for (auto each : rawKeyEnumMap)
        {
            if (each.first == wParam)
            {
                if (keyDownSet.find(each.second) == keyDownSet.end())
                    keyPushedSet.insert(each.second);
                keyDownSet.insert(each.second);
            }
        }
        break;
    case WM_LBUTTONUP:
    case WM_MBUTTONUP:
    case WM_RBUTTONUP:
        break;
    case WM_KEYUP:
        for (auto each : rawKeyEnumMap)
        {
            if (each.first == wParam)
            {
                keyDownSet.erase(each.second);
            }
        }
        break;
    }*/
    return 0;
}
bool D2DInput::m_isKeyDown(KeyCode keyCode)
{
    return keyDownSet.find(keyCode) != keyDownSet.end();
}
bool D2DInput::m_isKeyPushed(KeyCode keyCode)
{
    return keyPushedSet.find(keyCode) != keyPushedSet.end();
}
bool D2DInput::m_isKeyLifted(KeyCode keyCode)
{
    return keyLiftedSet.find(keyCode) != keyLiftedSet.end();
}

Vector2d D2DInput::getMouseWorldPosition()
{
    auto ret = getMousePositionNormalized();
    auto camRes = D2DCamera::GetMainD2DCamera()->GetScaledResolution();
    ret.x *= camRes.width;
    ret.y *= camRes.height;
    ret += D2DCamera::GetMainD2DCamera()->GetTransform()->GetWorldPosition();
    return ret;
}
