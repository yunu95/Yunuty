#include "Guardian_shooter.h"
#include "Button.h"

Button* Button::CreateButton(GameObject* parentPanel, const Vector2d& pos, const wstring& str, double width, double height, D2DText** textOut)
{
    Button* button = parentPanel->GetScene()->AddGameObject(parentPanel)->AddComponent<Button>();
    button->GetTransform()->position = pos;

    D2DRectangle* rect = button->GetGameObject()->AddComponent<D2DRectangle>();
    rect->color = D2D1::ColorF::White;
    rect->width = width;
    rect->height = height;
    auto text = button->GetGameObject()->AddGameObject()->AddComponent<D2DText>();
    if (textOut)
        *textOut = text;
    text->text = str;
    text->color = D2D1::ColorF::Black;
    text->width = width;
    text->height = height;
    text->fontSize = 25;
    BoxCollider2D* buttonCollider = button->GetGameObject()->AddComponent<BoxCollider2D>();
    buttonCollider->SetWidth(width);
    buttonCollider->SetHeight(height);
    button->buttonPanel = rect;
    button->toggleButton = false;
    return button;
}
Button* Button::AddDraggableButton(GameObject* parent, double radius)
{
    Button* button = parent->AddComponent<Button>();

    auto buttonCollider = button->GetGameObject()->AddComponent<CircleCollider2D>();
    buttonCollider->SetRadius(radius);
    button->toggleButton = false;
    button->onDrag = [button]()
    {
        button->GetTransform()->SetWorldPosition(D2DInput::getMouseWorldPosition());
    };
    return button;
}
Button* Button::AddDraggableButton(GameObject* parent, double width, double height,BoxCollider2D** colliderOut)
{
    Button* button = parent->AddComponent<Button>();

    auto buttonCollider = button->GetGameObject()->AddComponent<BoxCollider2D>();
    buttonCollider->SetWidth(width);
    buttonCollider->SetHeight(height);
    button->toggleButton = false;
    button->onDrag = [button]()
    {
        button->GetTransform()->SetWorldPosition(D2DInput::getMouseWorldPosition());
    };
    if (colliderOut)
        *colliderOut = buttonCollider;
    return button;
}
Button* Button::CreateToggleButton(GameObject* parentPanel, const Vector2d& pos, const wstring& str, double width, double height, D2DText** textOut)
{
    auto btn = CreateButton(parentPanel, pos, str, width, height, textOut);
    btn->toggleButton = true;
    return btn;
}
Button* Button::CreateToggleIconButton(GameObject* parentPanel, const Vector2d& pos, const wstring& animSpritePath, double width, double height)
{
    Button* button = parentPanel->GetScene()->AddGameObject(parentPanel)->AddComponent<Button>();
    button->GetTransform()->position = pos;

    D2DRectangle* rect = button->GetGameObject()->AddComponent<D2DRectangle>();
    rect->color = D2D1::ColorF::White;
    rect->width = width;
    rect->height = height;
    auto iconAnimation = button->GetGameObject()->AddGameObject()->AddComponent<D2DAnimatedSprite>();
    iconAnimation->LoadAnimationFromFile(animSpritePath);
    iconAnimation->SetIsRepeating(true);
    iconAnimation->Play();
    iconAnimation->SetWidth(width);
    iconAnimation->SetHeight(height);

    BoxCollider2D* buttonCollider = button->GetGameObject()->AddComponent<BoxCollider2D>();
    buttonCollider->SetWidth(width);
    buttonCollider->SetHeight(height);
    button->buttonPanel = rect;
    button->toggleButton = true;
    return button;
}
Button* Button::CreateToggleSimpleIconButton(GameObject* parentPanel, const Vector2d& pos, const wstring& spritePath, double width, double height)
{
    Button* button = parentPanel->GetScene()->AddGameObject(parentPanel)->AddComponent<Button>();
    button->GetTransform()->position = pos;

    D2DRectangle* rect = button->GetGameObject()->AddComponent<D2DRectangle>();
    rect->color = D2D1::ColorF::White;
    rect->width = width;
    rect->height = height;
    auto iconAnimation = button->GetGameObject()->AddGameObject()->AddComponent<D2DSprite>();
    iconAnimation->SetSpriteFilePath(spritePath);
    iconAnimation->SetDrawRect(Rect(width, height));

    BoxCollider2D* buttonCollider = button->GetGameObject()->AddComponent<BoxCollider2D>();
    buttonCollider->SetWidth(width);
    buttonCollider->SetHeight(height);
    button->buttonPanel = rect;
    button->toggleButton = true;
    return button;
}
Button::~Button()
{
    Cursor::ReportDead(this);
}
void Button::Update()
{
    if (selected && deselectOnESC && D2DInput::isKeyPushed(KeyCode::ESC))
        OnDeselected();

    onUpdate();
    if (selected)
        onUpdateWhileSelected();
}
void Button::OnMouseDrag()
{
    onDrag();
}
void Button::OnLeftClick()
{
    onClick();

    if (!toggleButton)
    {
        DebugObject::CreatePopUpCircle(GetTransform()->GetWorldPosition(), 10);
        return;
    }
    if (!selected)
        OnSelected();
    else
        OnDeselected();
}
void Button::OnSelected()
{
    selected = true;
    buttonPanel->color = D2D1::ColorF::DarkGray;
    onSelect();
    for (auto each : radioSelectGroup)
        if (each != this && each->selected)
            each->OnDeselected();
}
void Button::OnDeselected()
{
    selected = false;
    if (buttonPanel)
        buttonPanel->color = D2D1::ColorF::White;
    onDeselect();
}
void Button::OnMouseOver()
{
    if (buttonPanel)
        buttonPanel->color = selected ? D2D1::ColorF::DarkCyan : D2D1::ColorF::LightCyan;
    onMouseOver();
}
void Button::OnMouseExit()
{
    if (buttonPanel)
        buttonPanel->color = selected ? D2D1::ColorF::Gray : D2D1::ColorF::White;
    onMouseExit();
}
void Button::OnCollisionEnter2D(const Collision2D& collision)
{
    if (collision.m_OtherCollider->GetGameObject()->GetComponent<Cursor>())
    {
        OnMouseOver();
    }
}
void Button::OnCollisionExit2D(const Collision2D& collision)
{
    if (collision.m_OtherCollider->GetGameObject()->GetComponent<Cursor>())
    {
        OnMouseExit();
    }
}
//
//void Button::OnMouseOver()
//{
//    mouseOverCallable();
//}
//
//void Button::OnMouseExit()
//{
//    mouseExitCallable();
//}
//void Button::OnMouseDrag()
//{
//    mouseDragCallable();
//}
//
//void Button::OnLeftClick()
//{
//    leftClickCallable();
//}
//
//void Button::OnMiddleClick()
//{
//    middleClickCallable();
//}
//
//void Button::OnRightClick()
//{
//    rightClickCallable();
//}
//
//void Button::OnSelected()
//{
//    selectedCallable();
//}
//
//void Button::OnDeselected()
//{
//    deselectedCallable();
//}
//
//void Button::SetMouseOverCallable(const function<void()>& callable)
//{
//    mouseOverCallable = callable;
//}
//
//void Button::SetMouseExitCallable(const function<void()>& callable)
//{
//    mouseExitCallable = callable;
//}
//void Button::SetMouseDragCallable(const function<void()>& callable)
//{
//    mouseDragCallable = callable;
//}
//
//void Button::SetLeftClickCallable(const function<void()>& callable)
//{
//    leftClickCallable = callable;
//}
//
//void Button::SetMiddleClickCallable(const function<void()>& callable)
//{
//    middleClickCallable = callable;
//}
//
//void Button::SetRightClickCallable(const function<void()>& callable)
//{
//    rightClickCallable = callable;
//}
//
//void Button::SetSelectedCallable(const function<void()>& callable)
//{
//    selectedCallable = callable;
//}
//
//void Button::SetDeselectedCallable(const function<void()>& callable)
//{
//    deselectedCallable = callable;
//}
