#pragma once
#include "Component.h"
#include "Vector3.h"
#include "Quaternion.h"

#ifdef YUNUTY_EXPORTS
#define YUNUTY_API __declspec(dllexport)
#else
#define YUNUTY_API __declspec(dllimport)
#endif

namespace YunutyEngine
{
    class GameObject;
    class YUNUTY_API Transform :
        public Component
    {
    public:
        // 이 아래로 position, rotation, scale 이 셋은 계층구조 캐싱을 위해 Setter에 의해 감싸져야 합니다.
        Vector3d position = Vector3d();
        Quaternion rotation;
        Vector3d scale = Vector3d::one;
        Vector3d GetWorldPosition()const;
        Quaternion GetWorldRotation()const;
        void SetWorldPosition(const Vector3d& position);
        void SetWorldRotation(const Quaternion& rotation);
        Vector3d GetWorldScale()const;
    private:
        void SetCacheDirty();
    };
}
