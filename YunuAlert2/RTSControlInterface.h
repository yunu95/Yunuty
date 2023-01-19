#pragma once
#include "YunutyEngine.h"

class Unit;

class RTSControlInterface : public Component
{
private:
    unordered_set<Unit*> squads[10];
    float scalingDirection = 1;
    Vector2d dragStartPoint;
    enum class State
    {
        Neutral,
        Click,
        Dragging,
    };
protected:
    virtual void Update() override;
public:
    unordered_set<Unit*> selectedUnits;
    D2DSprite* selectRectangle = nullptr;
    D2DRectangle* dragRect = nullptr;
    BoxCollider2D* dragRectCollider = nullptr;
    D2DAnimatedSprite* rightClickAnimation = nullptr;
};