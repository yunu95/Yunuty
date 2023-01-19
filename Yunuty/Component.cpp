#include "YunutyEngine.h"
#include "Component.h"
#include "GameObject.h"
#include "Transform.h"

using namespace YunutyEngine;
YunutyEngine::GameObject* YunutyEngine::Component::GetGameObject()const
{
    return gameObject;
}
Transform* YunutyEngine::Component::GetTransform()const
{
    return gameObject->GetTransform();
}
