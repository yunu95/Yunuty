#pragma once
#include "YunutyEngine.h"

using namespace YunutyEngine;
class Ground : public Component
{
public:
    static const double groundHeight;
    static Ground* CreateGround(Vector2d position, double rotationRadian = 0, double length = 300, bool isPenetrable = false);
    bool isPenetrable;
    bool isSteppable()
    {
        return GetTransform()->rotation.Up().y > 0.4;
        //&&
            //Vector3d::Dot(footPosition - GetTransform()->GetWorldPosition(), GetTransform()->rotation.Up()) > 0;
    }
    bool isUnderFoot(Vector3d footPosition) { return Vector3d::Dot(footPosition - GetTransform()->GetWorldPosition(), GetTransform()->rotation.Up()) > 0; }
    BoxCollider2D* groundCollider;
protected:
    void OnCollisionStay2D(const Collision2D& colliision) override;
};

