#pragma once
#include "Object.h"
#include "EKeycode.h"
#include "GlobalComponent.h"
#include "Vector2.h"
#include <dinput.h>

// À¯¿ëÇÔ
// GetAsyncKeyState
#ifdef YUNUTY_EXPORTS
#define YUNUTY_API __declspec(dllexport)
#else
#define YUNUTY_API __declspec(dllimport)
#endif


namespace YunutyEngine
{
    class YUNUTY_API Input :
        public GlobalComponent
    {
    private:
        static unique_ptr<Input> instance;
    protected:
        //IDirectInput8* directInput;
        Input();
        virtual bool m_isKeyDown(KeyCode keyCode) = 0;
        virtual bool m_isKeyPushed(KeyCode keyCode) = 0;
        virtual bool m_isKeyLifted(KeyCode keyCode) = 0;
    public:
        static bool isKeyDown(KeyCode keyCode);
        static bool isKeyPushed(KeyCode keyCode);
        static bool isKeyLifted(KeyCode keyCode);
        static Vector2d getMousePosition();
        static Vector2d getMousePositionNormalized();
        static Vector3d getMouseWorldPosition();
    };
}
