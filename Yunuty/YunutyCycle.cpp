#include "YunutyEngine.h"
#include <iostream>
#include <chrono>
#include <stack>
#include "YunutyCycle.h"
#include "YunutyTime.h"
#include "GameObject.h"
#include "Component.h"
#include "GlobalComponent.h"
#include "Scene.h"
#include "Camera.h"
#include <cassert>

using namespace YunutyEngine;

YunutyEngine::YunutyCycle* YunutyEngine::YunutyCycle::_instance = nullptr;
YunutyEngine::YunutyCycle::YunutyCycle()
{
    assert(!_instance);
}
void YunutyEngine::YunutyCycle::Release()
{
    Stop();
    GlobalComponent::globalComponents.clear();
    for (auto& each : GlobalComponent::globalComponents)
        delete each;
}
YunutyEngine::YunutyCycle& YunutyEngine::YunutyCycle::GetInstance()
{
    if (!_instance)
        _instance = new YunutyCycle();
    return *_instance;
}
void YunutyEngine::YunutyCycle::Initialize()
{
}

void YunutyEngine::YunutyCycle::Play()
{
    updateThread = thread(&YunutyCycle::ThreadFunction, this);
    isGameRunning = true;
    ActiveComponentsDo(StartComponent);
}

void YunutyEngine::YunutyCycle::Stop()
{
    isGameRunning = false;
    updateThread.join();
}

void YunutyEngine::YunutyCycle::Pause()
{
}

void YunutyEngine::YunutyCycle::SetMaxFrameRate()
{
}

bool YunutyEngine::YunutyCycle::IsGameRunning()
{
    return isGameRunning;
}

void YunutyEngine::YunutyCycle::ThreadFunction()
{
    while (isGameRunning)
    {
        ThreadUpdate();
        std::this_thread::sleep_for(std::chrono::milliseconds(10));
    }
}
// Update components and render camera
void YunutyEngine::YunutyCycle::ThreadUpdate()
{
    Time::Update();

    for (auto i = GlobalComponent::globalComponents.begin(); i != GlobalComponent::globalComponents.end(); i++)
        (*i)->Update();
    //i->second->Update();

    ActiveComponentsDo(&Component::Update);

    Collider2D::InvokeCollisionEvents();

    if (Camera::mainCamera)
        Camera::mainCamera->Render();
}

void YunutyEngine::YunutyCycle::UpdateComponent(Component* component)
{
    if (!component->StartCalled)
    {
        component->StartCalled = true;
        component->Start();
    }
    component->Update();
}
void YunutyEngine::YunutyCycle::StartComponent(Component* component)
{
    component->StartCalled = true;
    component->Start();
}
void YunutyEngine::YunutyCycle::ActiveComponentsDo(function<void(Component*)> todo)
{
    for (auto each : GetActiveComponents())
        todo(each);
}
void YunutyEngine::YunutyCycle::ActiveComponentsDo(void (Component::* method)())
{
    for (auto each : GetActiveComponents())
        (each->*method)();
}
vector<Component*> YunutyEngine::YunutyCycle::GetActiveComponents()
{
    vector<Component*> ret;
    YunutyEngine::IGameObjectParent* gameObjectParent = Scene::getCurrentScene();
    if (Scene::getCurrentScene())
    {
        stack<GameObject*> objectsStack;
        auto firstChildren = Scene::getCurrentScene()->GetChildren();
        for (auto each = firstChildren.rbegin(); each != firstChildren.rend(); each++)
            objectsStack.push(*each);

        while (!objectsStack.empty())
        {
            auto gameObject = objectsStack.top();
            objectsStack.pop();
            if (!gameObject->GetSelfActive())
                continue;


            for (auto each = gameObject->components.begin(); each != gameObject->components.end(); each++)
                ret.push_back(each->first);

            for (auto each = gameObject->children.begin(); each != gameObject->children.end(); each++)
                objectsStack.push(each->first);
        }
    }
    return ret;
}
