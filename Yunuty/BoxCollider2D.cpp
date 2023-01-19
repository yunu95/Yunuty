#include "YunutyEngine.h"
using namespace YunutyMath;

bool BoxCollider2D::isOverlappingWith(Collider2D* other)
{
    return other->isOverlappingWith(this);
}
bool BoxCollider2D::isOverlappingWith(BoxCollider2D* other)
{
    return Collider2D::isOverlapping(this, other);
}
bool BoxCollider2D::isOverlappingWith(CircleCollider2D* other)
{
    return Collider2D::isOverlapping(this, other);
}
bool BoxCollider2D::isOverlappingWith(LineCollider2D* other)
{
    return Collider2D::isOverlapping(this, other);
}
void BoxCollider2D::SetWidth(double width)
{
    collisonRect.width = width;
}
void BoxCollider2D::SetHeight(double height)
{
    collisonRect.height = height;
}
double BoxCollider2D::GetWidth()const
{
    return collisonRect.width;
}
double BoxCollider2D::GetHeight()const
{
    return collisonRect.height;
}
Interval BoxCollider2D::projectedInterval(const Vector2d& vParam)const
{
    Vector2d v = vParam.Normalized();
    auto scale = GetTransform()->GetWorldScale();
    auto scaledHeight = collisonRect.height * scale.y;
    auto scaledWidth = collisonRect.width * scale.x;
    auto angle = GetTransform()->GetWorldRotation().Euler().z * YunutyMath::Deg2Rad;
    auto innerAngle = atan(scaledHeight / scaledWidth);
    auto r = Vector2d(scaledHeight, scaledWidth).Magnitude() / 2.0;

    auto projectedCenter = Vector2d::Dot(GetTransform()->GetWorldPosition(), v);
    auto projectedR = max(
        abs(Vector2d::Dot(Vector2d::DirectionByAngle(angle + innerAngle) * r, v)),
        abs(Vector2d::Dot(Vector2d::DirectionByAngle(angle - innerAngle) * r, v))
    );
    return Interval(projectedCenter, projectedR);
}
