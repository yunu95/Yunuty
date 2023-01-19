#pragma once
#include "Component.h"
#include "GameObject.h"
#include "Scene.h"
#include <functional>
#include <unordered_set>
#include <vector>

#ifdef YUNUTY_EXPORTS
#define YUNUTY_API __declspec(dllexport)
#else
#define YUNUTY_API __declspec(dllimport)
#endif

using namespace std;
using namespace YunutyEngine;
namespace YunutyEngine
{
    // ������Ʈ�� �ʿ��ϸ� �����ϰ�, ������ ������Ʈ�� �뵵�� �� ������ ����ϴ� ��� ��Ȱ��ȭ�� ���ѳ��ٰ�,
    // �ٽ� ������Ʈ�� ���� ��û�� ������ ��Ȱ��ȭ ��Ű�� ������Ʈ Ǯ ��ü�Դϴ�. 
    template<typename ReprenstativeComponent>
    class YUNUTY_API GameObjectPool : public Component
    {
        static_assert(std::is_base_of<Component, ReprenstativeComponent>::value, "only derived classes from component are allowed");
    public:
        // ������ ������Ʈ�� �� �ϳ��� ���� ���, ���ӿ�����Ʈ�� �׿� ���� �θ� ������Ʈ�� �����մϴ�.
        // �� ��, ������ ���� ������Ʈ�� �θ� ������Ʈ�� �Ű������� initializer�� �����մϴ�.
        void SetObjectInitializer(function<void(GameObject*, ReprenstativeComponent*)> initializer);
        // ���� ������Ʈ Ǯ�� ����� ���� ������Ʈ�� Ȱ��ȭ�մϴ�.
        ReprenstativeComponent* Borrow();
        // ���� ������Ʈ Ǯ���� �����ϴ� ���� ������Ʈ�� �ǵ��� �ݴϴ�.
        void Return(ReprenstativeComponent*);
    protected:
    private:
        unordered_set<ReprenstativeComponent*> poolObjects;
        unordered_set<ReprenstativeComponent*> expendableObjects;
        function<void(GameObject*, ReprenstativeComponent*)> objectInitializer = [](GameObject*) {};
    };
}
template<typename ReprenstativeComponent>
void GameObjectPool<ReprenstativeComponent>::SetObjectInitializer(function<void(GameObject*, ReprenstativeComponent*)> initializer)
{
    this->objectInitializer = initializer;
}
template<typename ReprenstativeComponent>
ReprenstativeComponent* GameObjectPool<ReprenstativeComponent>::Borrow()
{
    if (expendableObjects.empty())
    {
        auto gameObject = GetGameObject()->GetScene()->AddGameObject();
        auto component = gameObject->AddComponent<ReprenstativeComponent>();
        objectInitializer(gameObject, component);
        poolObjects.insert(component);
    }
    auto target = expendableObjects.begin();
    expendableObjects.erase(target);
    ((Component*)target)->GetGameObject()->SetSelfActive(true);
}
template<typename ReprenstativeComponent>
void GameObjectPool<ReprenstativeComponent>::Return(ReprenstativeComponent* obj)
{
    if (poolObjects.find(obj) != poolObjects.end())
    {
        expendableObjects.insert(obj);
        ((Component*)obj)->GetGameObject()->SetSelfActive(false);
    }
}
