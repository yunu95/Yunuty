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
    private:
    public:
        Vector3d position = Vector3d();
        Quaternion rotation;
        Vector3d scale = Vector3d::one;
        Vector3d GetWorldPosition()const;
        Quaternion GetWorldRotation()const;
        void SetWorldPosition(const Vector3d& position);
        void SetWorldRotation(const Quaternion& rotation);
        Vector3d GetWorldScale();
    };
}
