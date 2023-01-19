#include "YunutyEngine.h"
#include "Collision2D.h"

#include "Collider2D.h"

using namespace YunutyEngine;

unordered_set<Collider2D*> Collider2D::colliders2D;

Collider2D::Collider2D()
{
    colliders2D.insert(this);
}
Collider2D::~Collider2D()
{
    colliders2D.erase(this);
}
void Collider2D::InvokeCollisionEvents()
{
    for (auto collider1 : colliders2D)
        for (auto collider2 : colliders2D)
            if (collider2 != collider1)
                if (collider2->isOverlappingWith(collider1))
                {
                    Collision2D collision;
                    collision.m_Collider = collider1;
                    collision.m_Rigidbody = collider1->GetGameObject()->GetComponent<RigidBody2D>();
                    collision.m_OtherCollider = collider2;
                    collision.m_OtherRigidbody = collider2->GetGameObject()->GetComponent<RigidBody2D>();

                    if (collider1->overlappedColliders.find(collider2) == collider1->overlappedColliders.end())
                    {
                        collider1->overlappedColliders.insert(collider2);
                        for (auto each : collider1->GetGameObject()->GetComponents())
                            each->OnCollisionEnter2D(collision);
                    }
                    else
                    {
                        for (auto each : collider1->GetGameObject()->GetComponents())
                            each->OnCollisionStay2D(Collision2D());
                    }
                }
                else
                {
                    if (collider1->overlappedColliders.find(collider2) != collider1->overlappedColliders.end())
                    {
                        Collision2D collision;
                        collision.m_Collider = collider1;
                        collision.m_Rigidbody = collider1->GetGameObject()->GetComponent<RigidBody2D>();
                        collision.m_OtherCollider = collider2;
                        collision.m_OtherRigidbody = collider2->GetGameObject()->GetComponent<RigidBody2D>();

                        collider1->overlappedColliders.erase(collider2);
                        for (auto each : collider1->GetGameObject()->GetComponents())
                            each->OnCollisionExit2D(collision);
                    }
                }
}
bool Collider2D::isOverlapping(BoxCollider2D* a, BoxCollider2D* b)
{
    auto angleA = a->GetTransform()->GetWorldRotation().Euler().z * YunutyMath::Deg2Rad;
    auto angleB = b->GetTransform()->GetWorldRotation().Euler().z * YunutyMath::Deg2Rad;

    // 사각형의 각 변을 평면공간위에 표시한 벡터
    Vector2d horizontalAxisA(cos(angleA), sin(angleA));
    Vector2d verticalAxisA(-sin(angleA), cos(angleA));
    Vector2d horizontalAxisB(cos(angleB), sin(angleB));
    Vector2d verticalAxisB(-sin(angleB), cos(angleB));

    auto axes = { horizontalAxisA,verticalAxisA,horizontalAxisB,verticalAxisB };
    for (const Vector2d& each : axes)
    {
        Interval aInterval = a->projectedInterval(each);
        Interval bInterval = b->projectedInterval(each);
        if (!aInterval.OverlappedWith(bInterval))
            return false;
    }

    return true;
}
bool Collider2D::isOverlapping(BoxCollider2D* a, CircleCollider2D* b)
{
    auto angleA = a->GetTransform()->GetWorldRotation().Euler().z * YunutyMath::Deg2Rad;
    auto positionA = a->GetTransform()->GetWorldPosition();
    auto positionB = b->GetTransform()->GetWorldPosition();

    auto scale = a->GetTransform()->GetWorldScale();
    auto scaledHeight = a->collisonRect.height * scale.y;
    auto scaledWidth = a->collisonRect.width * scale.x;
    auto innerAngleA = atan(scaledHeight / scaledWidth);

    // 사각형의 각 변을 평면공간위에 표시한 벡터
    Vector2d horizontalAxisA(cos(angleA), sin(angleA));
    Vector2d verticalAxisA(-sin(angleA), cos(angleA));
    Vector2d rectPoints[4];
    Vector2d circleDisplacements[4];

    rectPoints[0] = positionA + horizontalAxisA * 0.5 * scaledWidth + verticalAxisA * 0.5 * scaledHeight;
    rectPoints[1] = positionA + horizontalAxisA * 0.5 * scaledWidth - verticalAxisA * 0.5 * scaledHeight;
    rectPoints[2] = positionA - horizontalAxisA * 0.5 * scaledWidth + verticalAxisA * 0.5 * scaledHeight;
    rectPoints[3] = positionA - horizontalAxisA * 0.5 * scaledWidth - verticalAxisA * 0.5 * scaledHeight;
    circleDisplacements[0] = rectPoints[0] - positionB;
    circleDisplacements[1] = rectPoints[1] - positionB;
    circleDisplacements[2] = rectPoints[2] - positionB;
    circleDisplacements[3] = rectPoints[3] - positionB;

    circleDisplacements[0] = circleDisplacements[0].Normalized();
    circleDisplacements[1] = circleDisplacements[1].Normalized();
    circleDisplacements[2] = circleDisplacements[2].Normalized();
    circleDisplacements[3] = circleDisplacements[3].Normalized();

    //vector<Vector2d> axes;
    //for (int i = 0; i < 360; i++)
        //axes.push_back(Vector2d::DirectionByAngle(i * YunutyMath::Deg2Rad));
    auto axes = { horizontalAxisA,verticalAxisA,circleDisplacements[0],circleDisplacements[1],circleDisplacements[2],circleDisplacements[3] };
    for (const Vector2d& each : axes)
    {
        Interval aInterval = a->projectedInterval(each);
        Interval bInterval = b->projectedInterval(each);
        if (!aInterval.OverlappedWith(bInterval))
            return false;
    }
    return true;
}
const unordered_set<Collider2D*>& Collider2D::GetOverlappedColliders() const
{
    return overlappedColliders;
}

bool Collider2D::isOverlapping(BoxCollider2D* a, LineCollider2D* b)
{
    return false;
}

bool Collider2D::isOverlapping(CircleCollider2D* a, CircleCollider2D* b)
{
    Vector2d aPos = a->GetTransform()->GetWorldPosition();
    Vector2d bPos = b->GetTransform()->GetWorldPosition();
    return (aPos - bPos).Magnitude() < a->radius + b->radius;
}

bool Collider2D::isOverlapping(CircleCollider2D* a, LineCollider2D* b)
{
    return false;
}

bool Collider2D::isOverlapping(LineCollider2D* a, LineCollider2D* b)
{
    return false;
}


