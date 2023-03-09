#include "MapTool.h"
#include "DebugObject.h"
#include "Cursor.h"
#include "CameraRail.h"

using namespace gs_map;

bool Cursor::touchingNothing = false;
Cursor* Cursor::instance = nullptr;

Cursor::Cursor()
{
    dragStartTimer.duration = 0.1;
    dragStartTimer.useScaledTime = false;
    if (!instance)
        instance = this;
}
Cursor::~Cursor()
{
    if (instance == this)
        instance = nullptr;
}
Cursor* Cursor::CreateCursor()
{
    auto cursorObj = Scene::getCurrentScene()->AddGameObject();
    auto cursor = cursorObj->AddComponent<Cursor>();
    cursorObj->AddComponent<CircleCollider2D>()->SetRadius(1);
    DebugObject::CreateDebugRectImage(cursorObj, 10, 10);
    return cursor;
}
void Cursor::ReportDead(Button* button)
{
    if (instance)
        instance->buttonList.erase(button);
}

//Cursor* Cursor::GetInstance()
//{
//    return instance;
//}
//void Cursor::OnCollisionEnter2D(const Collision2D& collision)
//{
//    auto btn = collision.m_OtherCollider->GetGameObject()->GetComponent<Button>();
//    if (btn)
//    {
//        buttonList.insert(btn);
//    }
//}
//void Cursor::OnCollisionExit2D(const Collision2D& collision)
//{
//    auto btn = collision.m_OtherCollider->GetGameObject()->GetComponent<Button>();
//    if (btn)
//    {
//        buttonList.erase(btn);
//    }
//}

void Cursor::Update()
{
    dragStartTimer.Update();
    touchingNothing = buttonList.empty();
    GetTransform()->SetWorldPosition(D2DInput::getMouseWorldPosition());
    if (D2DInput::isKeyDown(KeyCode::MouseLeftClick))
    {
        if (interactingButton && !dragStartTimer.isActive)
            interactingButton->OnMouseDrag();
    }
    if (D2DInput::isKeyLifted(KeyCode::MouseLeftClick))
    {
        interactingButton = nullptr;
    }

    if (buttonList.empty())
    {
        return;
    }

    if (D2DInput::isKeyPushed(KeyCode::MouseLeftClick))
    {
        if (!buttonList.empty())
        {
            for (auto each : buttonList)
                if (!interactingButton || interactingButton->GetGameObject()->GetSceneIndex() < each->GetGameObject()->GetSceneIndex())
                    interactingButton = each;
            interactingButton->OnLeftClick();
            dragStartTimer.Start();
        }
        else
        {
            interactingButton = nullptr;
        }
    }
    buttonList.clear();
}

void Cursor::OnCollisionEnter2D(const Collision2D& collision)
{
    Button* button = collision.m_OtherCollider->GetGameObject()->GetComponent<Button>();
    if (button == nullptr)
    {
        return;
    }
    button->OnMouseOver();
}
void Cursor::OnCollisionStay2D(const Collision2D& collision)
{
    Button* button = collision.m_OtherCollider->GetGameObject()->GetComponent<Button>();
    if (button == nullptr)
    {
        return;
    }
    buttonList.insert(button);
}

void Cursor::OnCollisionExit2D(const Collision2D& collision)
{
    Button* button = collision.m_OtherCollider->GetGameObject()->GetComponent<Button>();
    if (button == nullptr)
    {
        return;
    }
    button->OnMouseExit();
}


//
//Button* Cursor::interactingButton = nullptr;
//Cursor* Cursor::m_pInstance = nullptr;

