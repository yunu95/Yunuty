#pragma once
#include "IGameObjectParent.h"
#include <iostream>
#include "Object.h"
#include <unordered_map>
#include <unordered_set>

#ifdef YUNUTY_EXPORTS
#define YUNUTY_API __declspec(dllexport)
#else
#define YUNUTY_API __declspec(dllimport)
#endif


using namespace std;
namespace YunutyEngine
{
    class GameObject;
    class YunutyCycle;
    class YUNUTY_API Scene :
        public Object, public IGameObjectParent
    {
    private:
        string name;
        unordered_set<GameObject*> gameObjects;
        unordered_map<GameObject*, unique_ptr<GameObject>> children;
        vector<GameObject*> childrenIndexed;
        unordered_map<GameObject*, int> childIndexMap;
        static Scene* currentScene;
    protected:
        unique_ptr<YunutyEngine::GameObject> MoveChild(GameObject* child) override;
        void ReceiveChild(remove_reference<unique_ptr<GameObject>>::type&& child)override;
        int GetChildIndex(GameObject* child)override;
    public:
        Scene(const Scene&) = delete;
        Scene& operator=(const Scene&) = delete;
        Scene() = default;
        virtual const vector<GameObject*>& GetChildren()const override;
        static Scene* getCurrentScene();
        static void LoadScene(Scene* scene);
        string getName()const;
        void setName(const string& name);
        GameObject* AddGameObject(IGameObjectParent* parent = nullptr);
        GameObject* AddGameObject(string name, IGameObjectParent* parent = nullptr);
        friend YunutyCycle;
    };
}
