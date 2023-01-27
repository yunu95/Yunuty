#include "RTSControlInterface.h"
#include "YunuAlert2.h"

using namespace YunutyEngine;
void RTSControlInterface::Update()
{
    if (Input::isKeyPushed(KeyCode::MouseRightClick))
    {
        rightClickAnimation->Play();
        rightClickAnimation->GetTransform()->SetWorldPosition((Input::getMouseWorldPosition()));

        for (auto each : selectedUnits)
        {
            each->MoveCommand(Input::getMouseWorldPosition());
        }
    }
    if (Input::isKeyPushed(KeyCode::MouseLeftClick))
    {
        dragRect->GetGameObject()->SetSelfActive(true);
        dragStartPoint = Input::getMouseWorldPosition();
    }
    if (Input::isKeyDown(KeyCode::MouseLeftClick))
    {
        auto mouseCurrentPos = Input::getMouseWorldPosition();
        auto deltaPos = mouseCurrentPos - dragStartPoint;
        dragRect->GetTransform()->SetWorldPosition(Vector2d::Lerp(mouseCurrentPos, dragStartPoint, 0.5));
        dragRect->width = abs(deltaPos.x);
        dragRect->height = abs(deltaPos.y);
        dragRectCollider->SetHeight(abs(deltaPos.y));
        dragRectCollider->SetWidth(abs(deltaPos.x));
    }
    if (Input::isKeyLifted(KeyCode::MouseLeftClick))
    {
        dragRect->GetGameObject()->SetSelfActive(false);
        selectedUnits.clear();
        for (auto each : dragRectCollider->GetOverlappedColliders())
        {
            Unit* unit;
            if (unit = Unit::GetUnitFromCollider(each))
                selectedUnits.insert(unit);
        }
    }
}
