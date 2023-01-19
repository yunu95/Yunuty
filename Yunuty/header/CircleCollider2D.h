#pragma once
#include <Windows.h>
#include <vector>
#include <unordered_set>
#include "Graphic.h"
#include "Vector2.h"
#include "Vector3.h"
#include "Rect.h"
#include "Interval.h"
#include "D2DGraphic.h"

#ifdef YUNUTY_EXPORTS
#define YUNUTY_API __declspec(dllexport)
#else
#define YUNUTY_API __declspec(dllimport)
#endif

using namespace std;
namespace YunutyEngine
{
    class YUNUTY_API CircleCollider2D : public Collider2D
    {
        friend Collider2D;
    private:
        double radius;
    public:
        virtual bool isOverlappingWith(Collider2D* other) override;
        virtual bool isOverlappingWith(BoxCollider2D* other) override;
        virtual bool isOverlappingWith(CircleCollider2D* other) override;
        virtual bool isOverlappingWith(LineCollider2D* other) override;
        Interval projectedInterval(const Vector2d& v)const;
        void SetRadius(double radius);
    };
}