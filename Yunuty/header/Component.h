#pragma once
#include "Object.h"
//#include "GameObject.h"

#ifdef YUNUTY_EXPORTS
#define YUNUTY_API __declspec(dllexport)
#else
#define YUNUTY_API __declspec(dllimport)
#endif

namespace YunutyEngine
{
    class GameObject;
    class YunutyCycle;
    class Collision;
    class Collision2D;
    class Collider2D;
    class Collider;
    class Transform;

    /// <summary>
    /// Component는 게임 오브젝트들에게 배치될 수 있는 객체입니다.
    /// 게임 오브젝트는 해당 게임 오브젝트에 어떤 Component들이 배치되어 있느냐에 따라 그 성질이 정의된다고 볼 수 있습니다.
    /// </summary>
    class YUNUTY_API Component :
        public Object
    {
    private:
        GameObject* gameObject = nullptr;
        bool StartCalled = false;
    protected:
        Component();
    public:
        // 비활성화된 게임 오브젝트가 활성화 될 때 호출되는 콜백 함수입니다.
        virtual void OnEnable() {};
        // 활성화된 게임 오브젝트가 비활성화 될 때 호출되는 콜백 함수입니다.
        virtual void OnDisable() {};
        // 게임 실행 중 매 프레임마다 호출되는 콜백 함수입니다.
        virtual void Update() {};
        // 컴포넌트로부터 최초로 Update가 호출될 때, 호출되는 콜백 함수입니다.
        virtual void Start() {};
        // 게임 오브젝트가 삭제될 때 호출되는 콜백 함수입니다.
        virtual void OnDestroy() {};
        // 게임 실행 중 정확한 시간 간격으로 호출되는 콜백 함수입니다. 아직 구현되지 않았습니다.
        virtual void FixedUpdate() {};
        // 충돌체 간에 이벤트가 일어날 때, 콜라이더가 배치된 게임 오브젝트가 갖고 있는 모든 Component들에게
        // 해당 이벤트에 대응되는 콜백 함수가 호출됩니다.

        // 충돌체들이 서로 충돌을 시작할 때 호출되는 콜백 함수입니다.
        virtual void OnCollisionEnter(const Collision& collision) {};
        virtual void OnCollisionEnter2D(const Collision2D& collision) {};
        // 충돌체들이 서로 겹쳐진 상태로 있을때 매 프레임마다 호출되는 콜백 함수입니다.
        virtual void OnCollisionStay(const Collision& collision) {};
        virtual void OnCollisionStay2D(const Collision2D& collision) {};
        // 충돌체들이 충돌을 마치고 서로 떨어져 나갈때 호출되는 콜백 함수입니다.
        virtual void OnCollisionExit(const Collision& collision) {};
        virtual void OnCollisionExit2D(const Collision2D& collision) {};
        virtual ~Component() {}
        Transform* GetTransform();
        GameObject* GetGameObject();
        const Transform* GetTransform()const;
        const GameObject* GetGameObject()const;
        friend GameObject;
        friend YunutyCycle;
        friend Collider2D;
        friend Collider;
    };
}
