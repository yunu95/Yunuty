#include "YunutyEngine.h"
#include "Transform.h"
#include "GameObject.h"

using namespace YunutyEngine;

// ������ ����. ĳ�̿� �Ű��� ���� �ʾ� ���� ������ ���ݸ� ���������� ���ϰ� �ſ� Ŀ����.
Vector3d YunutyEngine::Transform::GetWorldPosition()const
{
    if (GetGameObject()->GetParentGameObject() == nullptr)
        return position;

    auto deltaPosition = (GetGameObject()->GetParentGameObject()->GetTransform()->GetWorldRotation() * position);
    auto parentScale = GetGameObject()->GetParentGameObject()->GetTransform()->GetWorldScale();
    deltaPosition.x *= parentScale.x;
    deltaPosition.y *= parentScale.y;
    deltaPosition.z *= parentScale.z;
    return  deltaPosition +
        GetGameObject()->GetParentGameObject()->GetTransform()->GetWorldPosition();
}
// ������ ����2
Quaternion YunutyEngine::Transform::GetWorldRotation()const
{
    if (GetGameObject()->GetParentGameObject() == nullptr)
        return rotation;
    return  GetGameObject()->GetParentGameObject()->GetTransform()->GetWorldRotation() * rotation;
}
Vector3d YunutyEngine::Transform::GetWorldScale()const
{
    if (GetGameObject()->GetParentGameObject() == nullptr)
        return scale;
    Vector3d ret;
    auto parentScale = GetGameObject()->GetParentGameObject()->GetTransform()->GetWorldScale();
    ret.x = scale.x * parentScale.x;
    ret.y = scale.y * parentScale.y;
    ret.z = scale.z * parentScale.z;
    return ret;
}
void YunutyEngine::Transform::SetWorldPosition(const Vector3d& position)
{
    auto parent = GetGameObject()->GetParentGameObject();

    if (!parent)
        this->position = position;
    else
        this->position = parent->GetTransform()->GetWorldRotation() * (position - parent->GetTransform()->GetWorldPosition());
}
// ������ ����4
void YunutyEngine::Transform::SetWorldRotation(const Quaternion& rotation)
{
    this->rotation = rotation;
}
void YunutyEngine::Transform::SetCacheDirty()
{

}