#include "YunutyEngine.h"
#include "GameObject.h"
#include "Component.h"
#include "Transform.h"
#include <stack>

using namespace YunutyEngine;
using namespace std;

YunutyEngine::GameObject::GameObject(IGameObjectParent* parent)
{
    transform = AddComponent<Transform>();
    SetParent(parent);
}
void YunutyEngine::GameObject::DoThingsOnParents(function<void(GameObject*)> todo)
{
    auto node = this->parentGameObject;
    while (node)
    {
        todo(node);
        node = node->parentGameObject;
    }
}
void YunutyEngine::GameObject::DeleteComponent(Component* component)
{
    components.erase(component);
}
bool YunutyEngine::GameObject::GetActive()
{
    if (!selfActive)
        return false;
    if (parentGameObject)
        return parentGameObject->GetActive();
    else
        return true;
}
bool YunutyEngine::GameObject::GetSelfActive()
{
    return selfActive;
}
void YunutyEngine::GameObject::SetSelfActive(bool selfActive)
{
    bool activeBefore = GetActive();
    bool activeAfter;

    this->selfActive = selfActive;

    activeAfter = GetActive();
    if (activeBefore != activeAfter)
    {
        if (activeAfter)
        {
            stack<GameObject*> activeStack;
            activeStack.push(this);
            while (!activeStack.empty())
            {
                auto child = activeStack.top();
                activeStack.pop();
                for (auto each : child->childrenIndexed)
                    if (each->selfActive)
                        activeStack.push(each);

                for (auto eachComp = child->components.begin(); eachComp != child->components.end(); eachComp++)
                    if (activeAfter)
                        eachComp->first->OnEnable();
                    else
                        eachComp->first->OnDisable();
            }
        }
    }

}
GameObject* YunutyEngine::GameObject::GetParentGameObject()
{
    return parentGameObject;
}
const GameObject* YunutyEngine::GameObject::GetParentGameObject()const
{
    return parentGameObject;
}
Scene* YunutyEngine::GameObject::GetScene()
{
    return scene;
}
void YunutyEngine::GameObject::SetParent(IGameObjectParent* parent)
{
    DoThingsOnParents([](GameObject* parent) {parent->childrenNum--; });
    if (this->parent)
        parent->ReceiveChild(this->parent->MoveChild(this));
    else
        parent->ReceiveChild(move(unique_ptr<GameObject>(this)));
    this->parent = parent;
    this->parentGameObject = dynamic_cast<GameObject*>(parent);
    DoThingsOnParents([](GameObject* parent) {parent->childrenNum++; });
}
// 이거 복잡도 n임.
unique_ptr<YunutyEngine::GameObject> YunutyEngine::GameObject::MoveChild(GameObject* child)
{
    auto ret = move(children[child]);
    auto index = GetChildIndex(child);
    children.erase(child);
    childIndexMap.erase(child);
    childrenIndexed.erase(childrenIndexed.begin() + index);
    for (int i = index; i < childrenIndexed.size(); i++)
        childIndexMap[childrenIndexed[i]] = i;

    return ret;
}

const vector<GameObject*>& YunutyEngine::GameObject::GetChildren()const
{
    return childrenIndexed;
}
void YunutyEngine::GameObject::ReceiveChild(remove_reference<unique_ptr<GameObject>>::type&& child)
{
    auto ptr = child.get();
    children.insert(make_pair(ptr, unique_ptr<GameObject>()));
    children[ptr] = move(child);
    childIndexMap.insert(make_pair(ptr, childrenIndexed.size()));
    childrenIndexed.push_back(ptr);
}
int YunutyEngine::GameObject::GetChildIndex(const GameObject* child)const
{
    auto found = childIndexMap.find(child);
    return found == childIndexMap.end() ? -1 : found->second;
}
YunutyEngine::GameObject::~GameObject()
{

}
Transform* YunutyEngine::GameObject::GetTransform()
{
    return transform;
}
const Transform* YunutyEngine::GameObject::GetTransform()const
{
    return transform;
}
GameObject* YunutyEngine::GameObject::AddGameObject()
{
    return scene->AddGameObject(this);
}
int YunutyEngine::GameObject::GetChildIndex()const
{
    return parent->GetChildIndex(this);
}
void YunutyEngine::GameObject::SetChildIndex(int index)
{
    parent->SetChildIndex(this, index);
}
void YunutyEngine::GameObject::SetChildIndex(GameObject* child, int index)
{
    if (index >= children.size())
        index = children.size() - 1;
    if (index < 0)
        index = 0;

    if (children.find(child) == children.end())
        return;
    auto origin=childIndexMap[child];
    if (origin == index)
        return;

    if (origin > index)
    {
        for (int i = origin; i > index; i--)
            childrenIndexed[i] = childrenIndexed[i-1];
        childrenIndexed[index] = child;

        for (int i = origin; i >= index; i--)
            childIndexMap[childrenIndexed[i]] = i;
    }
    else
    {
        for (int i = origin; i < index; i++)
            childrenIndexed[i] = childrenIndexed[i+1];
        childrenIndexed[index] = child;

        for (int i = origin; i <= index; i++)
            childIndexMap[childrenIndexed[i]] = i;
    }
}
int YunutyEngine::GameObject::GetSceneIndex()const
{
    return GetSceneIndex(this);
}
int YunutyEngine::GameObject::GetSceneIndex(const GameObject* target)
{
    if (!target->cachedSceneIndex.IsDirty())
        return target->cachedSceneIndex;

#if _DEBUG
    messyIndexingCalled++;
#endif
    stack<const GameObject*> objStack;
    objStack.push(target);
    while (!objStack.empty())
    {
        auto obj = objStack.top();
        if (!obj->cachedSceneIndex.IsDirty())
        {
            objStack.pop();
            if (!objStack.empty())
                objStack.top()->cachedSceneIndex = objStack.top()->cachedSceneIndex + obj->cachedSceneIndex;
            continue;
        }
        else
        {
            int childIndex = obj->parent->GetChildIndex(obj);
            if (childIndex == 0)
            {
                if (obj->parentGameObject)
                {
                    objStack.push(obj->parentGameObject);
                    obj->cachedSceneIndex = 1;
                    continue;
                }
                // 씬에서 가장 첫번째로 배치된 게임오브젝트
                else
                {
                    obj->cachedSceneIndex = 0;
                    continue;
                }
            }
            else
            {
                auto brother = obj->parent->GetChildren()[childIndex - 1];
                obj->cachedSceneIndex = brother->childrenNum + 1;
                objStack.push(brother);
            }
        }
    }
    return target->cachedSceneIndex;
    //int childIndex = target->parent->GetChildIndex(target);
    //if (childIndex == 0)
    //{
    //    if (target->parentGameObject)
    //        return GetSceneIndexRecursive(target->parentGameObject, deltaIndex + 1);
    //    else
    //        return deltaIndex;
    //}
    //else
    //{
    //    auto brother = target->parent->GetChildren()[childIndex - 1];
    //    ret += brother->GetSceneIndex() + brother->childrenNum;
    //}

    //return target->cachedSceneIndex = ret;
}
string YunutyEngine::GameObject::getName()const
{
    return name;
}
void YunutyEngine::GameObject::setName(const string& name)
{
    this->name = name;
}
void YunutyEngine::GameObject::SetCacheDirty()
{
    cachedSceneIndex.SetDirty();
}

#if _DEBUG
int YunutyEngine::GameObject::messyIndexingCalled = 0;
bool YunutyEngine::GameObject::addComponentFlag = false;
#endif

