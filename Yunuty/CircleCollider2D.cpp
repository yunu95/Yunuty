#include "YunutyEngine.h"

using namespace YunutyEngine;

bool CircleCollider2D::isOverlappingWith(Collider2D* other)
{
    return other->isOverlappingWith(this);
}
bool CircleCollider2D::isOverlappingWith(BoxCollider2D* other)
{
    return Collider2D::isOverlapping(other, this);
}
bool CircleCollider2D::isOverlappingWith(CircleCollider2D* other)
{
    return Collider2D::isOverlapping(other, this);
}
bool CircleCollider2D::isOverlappingWith(LineCollider2D* other)
{
    return Collider2D::isOverlapping(other, this);
}
void CircleCollider2D::SetRadius(double radius)
{
    this->radius = radius;
}
Interval CircleCollider2D::projectedInterval(const Vector2d& v)const
{
    auto projectedCenter = Vector2d::Dot(GetTransform()->GetWorldPosition(), v);
    return Interval(projectedCenter, radius * GetTransform()->GetWorldScale().x);
}