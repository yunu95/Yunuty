#include "YunutyEngine.h"
#include "Component.h"
#include "GameObject.h"
#include "Transform.h"

using namespace YunutyEngine;
Component::Component()
{
#if _DEBUG
	if (GameObject::addComponentFlag != true)
	{
		_wassert(_CRT_WIDE("derived class of component class can only be created through the \"AddComponent\" function, which is the member function of gameobject class instance."), _CRT_WIDE(__FILE__), (unsigned)(__LINE__));
	}
	GameObject::addComponentFlag = false;
#endif
}
YunutyEngine::GameObject* YunutyEngine::Component::GetGameObject()
{
	return gameObject;
}
const YunutyEngine::GameObject* YunutyEngine::Component::GetGameObject()const
{
	return gameObject;
}
Transform* YunutyEngine::Component::GetTransform()
{
	return gameObject->GetTransform();
}
const Transform* YunutyEngine::Component::GetTransform()const
{
	return gameObject->GetTransform();
}
