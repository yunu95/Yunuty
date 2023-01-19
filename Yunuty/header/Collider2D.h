#pragma once
#include <Windows.h>
#include <vector>
#include <unordered_set>
#include "Component.h"
#include "Vector2.h"
#include "Vector3.h"
#include "Rect.h"
#include "D2DGraphic.h"
#include "Interval.h"

#ifdef YUNUTY_EXPORTS
#define YUNUTY_API __declspec(dllexport)
#else
#define YUNUTY_API __declspec(dllimport)
#endif


using namespace std;
namespace YunutyEngine
{
    class BoxCollider2D;
    class CircleCollider2D;
    class LineCollider2D;
    class RigidBody2D;
    class YUNUTY_API Collider2D abstract : public Component
    {
        // 쿼드트리 최적화 나중에 구현하기
    private:
        // called by yunuty cycle
        static void InvokeCollisionEvents();
    protected:
        static unordered_set<Collider2D*> colliders2D;
        unordered_set<Collider2D*> overlappedColliders;

        Collider2D();
        virtual ~Collider2D();

        static bool isOverlapping(BoxCollider2D* a, BoxCollider2D* b);
        static bool isOverlapping(BoxCollider2D* a, CircleCollider2D* b);
        static bool isOverlapping(CircleCollider2D* b, BoxCollider2D* a) { return isOverlapping(a, b); }
        static bool isOverlapping(BoxCollider2D* a, LineCollider2D* b);
        static bool isOverlapping(LineCollider2D* b, BoxCollider2D* a) { return isOverlapping(a,b); }
        static bool isOverlapping(CircleCollider2D* a, CircleCollider2D* b);
        static bool isOverlapping(CircleCollider2D* a, LineCollider2D* b);
        static bool isOverlapping(LineCollider2D* b, CircleCollider2D* a) { return isOverlapping(a,b); }
        static bool isOverlapping(LineCollider2D* a, LineCollider2D* b);

    public:
        const unordered_set<Collider2D*>& GetOverlappedColliders() const;
        virtual bool isOverlappingWith(Collider2D* other) = 0;
        virtual bool isOverlappingWith(BoxCollider2D* other) = 0;
        virtual bool isOverlappingWith(CircleCollider2D* other) = 0;
        virtual bool isOverlappingWith(LineCollider2D* other) = 0;
        friend YunutyCycle;
    };
}