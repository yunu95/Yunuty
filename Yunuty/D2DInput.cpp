#include "YunuD2DGraphicCore.h"
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
    rawKeyEnumMap.insert(make_pair('C', KeyCode::C));
    rawKeyEnumMap.insert(make_pair('D', KeyCode::D));
    rawKeyEnumMap.insert(make_pair('E', KeyCode::E));
    rawKeyEnumMap.insert(make_pair('F', KeyCode::F));
    rawKeyEnumMap.insert(make_pair('G', KeyCode::G));
    rawKeyEnumMap.insert(make_pair('H', KeyCode::H));
    rawKeyEnumMap.insert(make_pair('I', KeyCode::I));
    rawKeyEnumMap.insert(make_pair('J', KeyCode::J));
    rawKeyEnumMap.insert(make_pair('K', KeyCode::K));
    rawKeyEnumMap.insert(make_pair('L', KeyCode::L));
    rawKeyEnumMap.insert(make_pair('M', KeyCode::M));
    rawKeyEnumMap.insert(make_pair('N', KeyCode::N));
    rawKeyEnumMap.insert(make_pair('O', KeyCode::O));
    rawKeyEnumMap.insert(make_pair('P', KeyCode::P));
    rawKeyEnumMap.insert(make_pair('Q', KeyCode::Q));
    rawKeyEnumMap.insert(make_pair('R', KeyCode::R));
    rawKeyEnumMap.insert(make_pair('S', KeyCode::S));
    rawKeyEnumMap.insert(make_pair('T', KeyCode::T));
    rawKeyEnumMap.insert(make_pair('U', KeyCode::U));
    rawKeyEnumMap.insert(make_pair('V', KeyCode::V));
    rawKeyEnumMap.insert(make_pair('W', KeyCode::W));
    rawKeyEnumMap.insert(make_pair('X', KeyCode::X));
    rawKeyEnumMap.insert(make_pair('Y', KeyCode::Y));
    rawKeyEnumMap.insert(make_pair('Z', KeyCode::Z));
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
    ret.x -= 0.5;
    ret.y -= 0.5;
    ret.y *= -1;
    auto camRes = D2DCamera::GetMainD2DCamera()->GetResolution();
    ret.x *= camRes.width * D2DCamera::GetMainD2DCamera()->GetZoomOutFactor();
    ret.y *= camRes.height * D2DCamera::GetMainD2DCamera()->GetZoomOutFactor();
    ret += D2DCamera::GetMainD2DCamera()->GetTransform()->GetWorldPosition();
    return ret;
}
