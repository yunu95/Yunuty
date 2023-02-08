#include "YunutyEngine.h"
#include "ShakyFella.h"

void ShakyFella::OnCollisionEnter2D(const Collision2D& collision)
{
    Vector3d displacement = GetTransform()->GetWorldPosition() - collision.m_OtherCollider->GetTransform()->GetWorldPosition();

    if (GetGameObject()->GetComponent<YunutyEngine::D2D::D2DRectangle>())
        GetGameObject()->GetComponent<YunutyEngine::D2D::D2DRectangle>()->color = D2D1::ColorF::Yellow;

    if (GetGameObject()->GetComponent<YunutyEngine::D2D::D2DCircle>())
        GetGameObject()->GetComponent<YunutyEngine::D2D::D2DCircle>()->color = D2D1::ColorF::Yellow;
}
void ShakyFella::OnCollisionExit2D(const Collision2D& collision)
{
    if (GetGameObject()->GetComponent<YunutyEngine::Collider2D>()->GetOverlappedColliders().size() > 0)
        return;

    if (GetGameObject()->GetComponent<YunutyEngine::D2D::D2DRectangle>())
        GetGameObject()->GetComponent<YunutyEngine::D2D::D2DRectangle>()->color = D2D1::ColorF::Brown;

    if (GetGameObject()->GetComponent<YunutyEngine::D2D::D2DCircle>())
        GetGameObject()->GetComponent<YunutyEngine::D2D::D2DCircle>()->color = D2D1::ColorF::Brown;
}
void ShakyFella::Update()
{
    //if (!GetGameObject()->GetParentGameObject())
    //{
    //    Vector2d inputVector;

    //    if (Input::isKeyDown(KeyCode::DownArrow))
    //        inputVector += Vector2d::down;
    //    if (Input::isKeyDown(KeyCode::UpArrow))
    //        inputVector += Vector2d::up;
    //    if (Input::isKeyDown(KeyCode::LeftArrow))
    //        inputVector += Vector2d::left;
    //    if (Input::isKeyDown(KeyCode::RightArrow))
    //        inputVector += Vector2d::right;

    //    inputVector = inputVector.Normalized();
    //    GetGameObject()->GetComponent<RigidBody2D>()->velocity += 250 * inputVector * Time::GetDeltaTime();
    //    GetTransform()->scale.x += scalingDirection * shakeSpeed * Time::GetDeltaTime();
    //    GetTransform()->scale.y += scalingDirection * shakeSpeed * Time::GetDeltaTime();
    //}
    if (GetTransform()->scale.x > maxSize)
    {
        scalingDirection = -1;
        GetTransform()->scale.x = GetTransform()->scale.y = maxSize;
    }
    if (GetTransform()->scale.x < minSize)
        GetTransform()->scale.x = GetTransform()->scale.y = maxSize;

    if (Input::isKeyPushed(KeyCode::MouseLeftClick))
    {
        auto pos = D2DInput::getMouseWorldPosition();
        auto gobj = GetGameObject()->GetScene()->AddGameObject();
        auto rect = gobj->AddComponent<D2DRectangle>();
        rect->height = 10;
        rect->width = 10;
        rect->color = D2D1::ColorF::Gray;
        gobj->GetTransform()->SetWorldPosition(pos);
    }

    scalingDirection = 1;
}
