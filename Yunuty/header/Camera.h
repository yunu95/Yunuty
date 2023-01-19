#pragma once
#include "Object.h"
#include "Component.h"
#include "Rect.h"

// ī�޶�� ȭ�鿡 ���ӻ� �����ϴ� ��� �׷��� ��ҵ��� ����ϴ� ���� ���̴� ��ü�Դϴ�. 
// ī�޶��� ������ ������ ����ȯ��,Ȥ�� ī�޶��� Ư���� ���� �޶��� �� �ֱ� ������,
// ȭ���� �׷����� ���� ���̴� �Լ� Render�� �߻� �޼ҵ�� ���ǵ˴ϴ�.
#ifdef YUNUTY_EXPORTS
#define YUNUTY_API __declspec(dllexport)
#else
#define YUNUTY_API __declspec(dllimport)
#endif

namespace  YunutyEngine
{
    class YunutyCycle;
    class YUNUTY_API Camera :
        public Component
    {
    private:
        static Camera* mainCamera;
        Rect nearPlane;
    protected:
        Camera();
        virtual void Render() = 0;
    public:
        void SetCameraMain();
        static Camera* GetMainCamera();
        virtual void SetNearPlane(const Rect& plane);
        Rect GetNearPlane();
        friend YunutyCycle;
    };
}
