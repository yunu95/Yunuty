#pragma once
#include "YunutyEngine.h"

class Button :
    public Component
{
public:
    virtual ~Button();
    virtual void Update() override;
    static Button* CreateButton(GameObject* parentPanel, const Vector2d& pos, const wstring& str, double width = 250, double height = 75, D2DText** text = nullptr);
    static Button* CreateToggleButton(GameObject* parentPanel, const Vector2d& pos, const wstring& str, double width = 250, double height = 75, D2DText** textOut=nullptr);
    static Button* CreateToggleIconButton(GameObject* parentPanel, const Vector2d& pos, const wstring& animSpritePath, double width = 150, double height = 150);
    static Button* CreateToggleSimpleIconButton(GameObject* parentPanel, const Vector2d& pos, const wstring& spritePath, double width = 150, double height = 150);
    static Button* AddDraggableButton(GameObject* parent, double radius = 50);
    static Button* AddDraggableButton(GameObject* parent, double width,double height,BoxCollider2D** colliderOut);

    function<void(void)> onClick = []() {};
    function<void(void)> onDrag = []() {};
    function<void(void)> onSelect = []() {};
    function<void(void)> onDeselect = []() {};
    function<void(void)> onEnable = []() {};
    function<void(void)> onMouseOver = []() {};
    function<void(void)> onMouseExit = []() {};
    function<void(void)> onUpdate = []() {};
    function<void(void)> onUpdateWhileSelected = []() {};
    virtual void OnMouseDrag();
    virtual void OnLeftClick();
    virtual void OnSelected();
    virtual void OnDeselected();
    virtual void OnMouseOver();
    virtual void OnMouseExit();

    virtual void OnEnable() { onEnable(); }
    virtual void OnCollisionEnter2D(const Collision2D& collision);
    virtual void OnCollisionExit2D(const Collision2D& collision);
    //void OnMiddleClick();
    //void OnRightClick();

    bool toggleButton = false;
    bool selected = false;
    bool deselectOnESC=false;
    vector<Button*> radioSelectGroup;

    struct Comp
    {
        bool operator()(Button* const input1, Button* const input2)const
        {
            return input1->GetGameObject()->GetSceneIndex() < input2->GetGameObject()->GetSceneIndex();
        }
    };
private:
    D2DRectangle* buttonPanel = nullptr;
    /*function<void(void)> mouseOverCallable = []() {};
    function<void(void)> mouseExitCallable = []() {};
    function<void(void)> mouseDragCallable = []() {};
    function<void(void)> leftClickCallable = []() {};
    function<void(void)> middleClickCallable = []() {};
    function<void(void)> rightClickCallable = []() {};
    function<void(void)> selectedCallable = []() {};
    function<void(void)> deselectedCallable = []() {};*/
};


