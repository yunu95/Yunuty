#include "YunutyEngine.h"
#include "Scene.h"
#include "GameObject.h"

using namespace YunutyEngine;
YunutyEngine::Scene* YunutyEngine::Scene::currentScene = nullptr;
YunutyEngine::Scene* YunutyEngine::Scene::getCurrentScene()
{
    return currentScene;
}
void YunutyEngine::Scene::LoadScene(Scene* scene)
{
    currentScene = scene;
}
string YunutyEngine::Scene::getName()const
{
    return name;
}
void YunutyEngine::Scene::setName(const string& name)
{
    this->name = name;
}
YunutyEngine::GameObject* YunutyEngine::Scene::AddGameObject(IGameObjectParent* parent)
{
    auto gameObject = new GameObject(parent != nullptr ? parent : this);
    gameObject->scene = this;
    /*if (!parent)
        gameObjects.insert(make_pair<gameObject, unique_ptr<GameObject>(gameObject));*/

    return gameObject;
}
YunutyEngine::GameObject* YunutyEngine::Scene::AddGameObject(string name, IGameObjectParent* parent)
{
    auto gameObject = AddGameObject(parent);
    gameObject->name = name;
    return gameObject;
}
unique_ptr<YunutyEngine::GameObject> YunutyEngine::Scene::MoveChild(GameObject* child)
{
    unique_ptr<YunutyEngine::GameObject> ret = move(children[child]);
    children.erase(child);
    auto erasedIndex = childIndexMap[child];
    childIndexMap.erase(child);
    childrenIndexed.erase(childrenIndexed.begin() + erasedIndex);
    for (auto& each : childIndexMap)
        if (each.second > erasedIndex)
            each.second--;
    return ret;
}

const vector<YunutyEngine::GameObject*>& YunutyEngine::Scene::GetChildren() const
{
    return childrenIndexed;
}
void YunutyEngine::Scene::ReceiveChild(remove_reference<unique_ptr<GameObject>>::type&& child)
{
    auto ptr = child.get();
    children.insert(make_pair(ptr, unique_ptr<GameObject>()));
    children[ptr] = move(child);
    children.emplace(ptr, move(child));
    childIndexMap.insert(make_pair(ptr, childrenIndexed.size()));
    childrenIndexed.push_back(ptr);
}
int YunutyEngine::Scene::GetChildIndex(const GameObject* child)const
{
    return childIndexMap.find(child)->second;
}
void YunutyEngine::Scene::SetChildIndex(GameObject* child, int index)
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
void YunutyEngine::Scene::DestroyGameObject(GameObject* gameObj)
{
    if (!gameObj)
        return;
    destroyList.insert(gameObj);
}
