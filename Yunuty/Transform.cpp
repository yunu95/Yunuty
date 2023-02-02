#include "YunutyEngine.h"
#include "Transform.h"
#include "GameObject.h"

using namespace YunutyEngine;

// 문제가 많다.
Vector3d YunutyEngine::Transform::GetWorldPosition()const
{
    if (GetGameObject()->GetParentGameObject() == nullptr)
        return position;

    return GetGameObject()->GetParentGameObject()->GetTransform()->GetWorldRotation() * position +
        GetGameObject()->GetParentGameObject()->GetTransform()->GetWorldPosition();
}
// 문제가 많다2
Quaternion YunutyEngine::Transform::GetWorldRotation()const
{
    if (GetGameObject()->GetParentGameObject() == nullptr)
        return rotation;
    return rotation * GetGameObject()->GetParentGameObject()->GetTransform()->GetWorldRotation();
}
Vector3d YunutyEngine::Transform::GetWorldScale()const
{
    if (GetGameObject()->GetParentGameObject() == nullptr)
        return scale;
    Vector3d ret;
    ret.x = scale.x * GetGameObject()->GetParentGameObject()->GetTransform()->GetWorldScale().x;
    ret.y = scale.y * GetGameObject()->GetParentGameObject()->GetTransform()->GetWorldScale().y;
    return ret;
}
void YunutyEngine::Transform::SetWorldPosition(const Vector3d& position)
{
    auto parent = GetGameObject()->GetParentGameObject();

    if (!parent)
        this->position = position;
    else
        this->position = GetWorldRotation().inverse() * (position - parent->GetTransform()->GetWorldPosition());
}
// 문제가 많다4
void YunutyEngine::Transform::SetWorldRotation(const Quaternion& rotation)
{
    this->rotation = rotation;
}