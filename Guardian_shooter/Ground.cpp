#include "Guardian_shooter.h"
#include "PatrolCollider.h"
using namespace YunutyEngine;

const double Ground::groundHeight = 20;

void Ground::OnCollisionStay2D(const Collision2D& collision)
{
    auto player = collision.m_OtherCollider->GetGameObject()->GetComponent<Player>();
    auto enemy = collision.m_OtherCollider->GetGameObject()->GetComponent<Threat>();
    auto position = GetTransform()->GetWorldPosition();
    auto rotation = GetTransform()->GetWorldRotation();
    if (!enemy && !player)
        return;
    if (player &&
        player->GetMovementState() == PlayerMovementState::JUMP &&
        Vector3d::Dot(player->GetPlayerSpeed(), GetTransform()->GetWorldRotation().Up()) > 0)
        return;
    if (player &&
        isPenetrable &&
        (player->GetMovementState() == PlayerMovementState::JUMP ||
            (!isUnderFoot(player->GetPlayerFootPos()))&& !isUnderFoot(player->lastFramePlayerPos)))
    {
        return;
    }
    if (player &&
        abs(Vector3d::Dot(player->GetPlayerFootPos() - position, rotation.Right())) > groundCollider->GetWidth() * 0.5
        )
    {
        return;
    }

    // 아래의 코드를 통해 지형이 플레이어나 적을 지형 반대방향으로 밀어냅니다.
    auto otherTransform = collision.m_OtherCollider->GetTransform();
    BoxCollider2D* otherBoxCollider = collision.m_OtherCollider->GetGameObject()->GetComponent<BoxCollider2D>();
    Vector3d delta;
    Vector3d delta_norm = Vector2d::DirectionByAngle((GetTransform()->GetWorldRotation().Euler().z + 90) * YunutyMath::Deg2Rad);
    //Vector3d delta_norm2 = GetTransform()->GetWorldRotation().Euler().;
    Vector3d otherBox_DeltaPoint = 0.5 * Vector3d(otherBoxCollider->GetWidth(), -otherBoxCollider->GetHeight(), 0);
    if (GetTransform()->GetWorldRotation().Up().x > 0)
        otherBox_DeltaPoint.x *= -1;
    Vector3d centerDelta = otherTransform->GetWorldPosition() - GetTransform()->GetWorldPosition();
    // 살짝 파고 들게 하기 위한 식
    const double diggingRate = 0.9;
    delta = delta_norm * ((groundCollider->GetHeight() * 0.5 + abs(Vector3d::Dot(otherBox_DeltaPoint, delta_norm))) * diggingRate - abs(Vector3d::Dot(centerDelta, delta_norm)));
    otherTransform->SetWorldPosition(otherTransform->GetWorldPosition() + delta);
}
Ground* Ground::CreateGround(Vector2d position, double rotationRadian, double length, bool isPenetrable)
{
    auto gameObject = Scene::getCurrentScene()->AddGameObject(gs_map::MapTool::GetInstance()->GetGroundLayer());
    gameObject->GetTransform()->position = position;
    gameObject->GetTransform()->rotation = Vector3d(0, 0, rotationRadian * YunutyMath::Rad2Deg);
    auto collider = gameObject->AddComponent<BoxCollider2D>();
    collider->SetWidth(length);
    collider->SetHeight(groundHeight);
    //auto debugGraphic = gameObject->AddComponent<YunutyEngine::D2D::D2DRectangle>();
    DebugObject::CreateColliderImage(collider, isPenetrable ? D2D1::ColorF::YellowGreen : D2D1::ColorF::Brown);
    //DebugObject::CreateArrow(gameObject, gameObject->GetTransform()->GetWorldPosition(), gameObject->GetTransform()->GetWorldPosition() + 20 * Vector2d::DirectionByAngle(rotationRadian * YunutyMath::Rad2Deg + 90));
    DebugObject::CreateArrow(gameObject, gameObject->GetTransform()->GetWorldPosition(), gameObject->GetTransform()->GetWorldPosition() + Vector2d::DirectionByAngle(90 * YunutyMath::Deg2Rad) * 20);
    auto ground = gameObject->AddComponent<Ground>();
    ground->isPenetrable = isPenetrable;
    ground->groundCollider = collider;
    //auto patrolBox1 = PatrolCollider::CreatePatrolbox();
    //patrolBox1->GetTransform()->position = Vector2d(position.x - 50 - (length / 2), position.y);
    //auto patrolBox2 = PatrolCollider::CreatePatrolbox();
    //patrolBox2->GetTransform()->position = Vector2d(position.x + 50 + (length / 2), position.y);
    return ground;
}
