#pragma once
#include "YunutyEngine.h"

class DebugObject : public Component
{
public:
    DebugObject();
    ~DebugObject();
    static void EnableDebugmode();
    static void DisableDebugmode();
    static void ToggleDebugmode();
    static void CreateDebugRectImage(GameObject* parent, double width, double height, D2D1::ColorF color = D2D1::ColorF::Yellow, double border = 3, bool filled = false);
    static void CreateColliderImage(BoxCollider2D* collider,  D2D1::ColorF color = D2D1::ColorF::Yellow, double border = 3, bool filled = false);
    static void CreateDebugCircleImage(GameObject* parent, double radius,  D2D1::ColorF color = D2D1::ColorF::Yellow, double border = 3, bool filled = false);
    static void CreateDebugText(GameObject* parent, function<wstring()>, Vector3d relativePosition = Vector3d::zero, double fontSize = 20, D2D1::ColorF color = D2D1::ColorF::Black);
    static void CreateArrow(GameObject* parent, Vector3d origin, Vector3d destination, double width = 3, D2D1::ColorF color = D2D1::ColorF::Yellow);
    static void CreateArrow(GameObject* origin, GameObject* destination, double width = 3, D2D1::ColorF color = D2D1::ColorF::Yellow);
    static void CreatePopUpCircle(GameObject* parent, double radius = 50, double duration = 0.5, D2D1::ColorF color = D2D1::ColorF::Yellow);
    static void CreatePopUpCircle(Vector3d position, double radius = 50, double duration = 0.5, D2D1::ColorF color = D2D1::ColorF::Yellow);
protected:
    void Update() override;
    void OnDestroy() override;
private:
    static void _CreatePopUpCircle(GameObject* parent, Vector3d position, double radius, double duration, D2D1::ColorF color);
    static tuple<GameObject*, DebugObject*, D2DRectangle* ,GameObject*,GameObject*> _CreateArrow(double lineLength, double width = 3, D2D1::ColorF color = D2D1::ColorF::Yellow);
    static bool debugMode;
    static unordered_set<DebugObject*> debugObjects;
    function<void()> onUpdate = []() {};
    function<void()> onDebugEnabled = []() {};
    function<void()> onDebugDisabled = []() {};
    function<void()> onDestory = []() {};

};
