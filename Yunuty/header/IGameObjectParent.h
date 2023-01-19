#pragma once
#include <iostream>
#include <vector>

#ifdef YUNUTY_EXPORTS
#define YUNUTY_API __declspec(dllexport)
#else
#define YUNUTY_API __declspec(dllimport)
#endif

using namespace std;
namespace YunutyEngine
{
    class GameObject;
    class Scene;

    class IGameObjectParent
    {
     private:
        virtual unique_ptr<YunutyEngine::GameObject> MoveChild(GameObject* child) = 0;
        virtual void ReceiveChild(remove_reference<unique_ptr<GameObject>>::type&& child) = 0;
        virtual int GetChildIndex(GameObject* child) = 0;
    public:
        virtual const vector<GameObject*>& GetChildren()const = 0;
        friend GameObject;
        friend Scene;
    };
}
