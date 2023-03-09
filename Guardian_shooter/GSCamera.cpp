#include "GSCamera.h"
#include "Guardian_shooter.h"
#include "CameraRail.h"
#include <algorithm>

GSCamera* GSCamera::instance = nullptr;

void GSCamera::Initialize()
{
    BoxCollider2D* collider = this->GetGameObject()->AddComponent<BoxCollider2D>();
    collider->SetWidth(GetResolution().width);
    collider->SetHeight(GetResolution().height);
    auto debugRectObj = Scene::getCurrentScene()->AddGameObject();
    DebugObject::CreateDebugRectImage(debugRectObj, 1920, 1080, D2D1::ColorF::Orange);
    debugRectObj->AddComponent<CameraSticky>()->followScale = false;
    GlobalReference::cameraRectCollider = collider;
}

void GSCamera::Restart()
{
    recoveryTimer.isActive = false;
    optimalPosition = CameraRail::firstRail->startVector;
    GetTransform()->SetWorldPosition(optimalPosition);
    lastCamPoint = GetTransform()->GetWorldPosition();
    threatsLegacy.clear();
}

void GSCamera::LerpToRailPoint()
{

}


void GSCamera::Update()
{
    recoveryTimer.Update();
    if (gs_map::MapTool::GetInstance()->isEditingMap())
    {
        Vector3d dir = Vector3d::zero;
        if (gs_map::TextInput::isInTypeMode())
            return;
        if (D2DInput::isKeyDown(KeyCode::W))dir += Vector3d::up;
        if (D2DInput::isKeyDown(KeyCode::A))dir += Vector3d::left;
        if (D2DInput::isKeyDown(KeyCode::S))dir += Vector3d::down;
        if (D2DInput::isKeyDown(KeyCode::D))dir += Vector3d::right;
        if (D2DInput::isKeyDown(KeyCode::Q))
        {
            SetZoomOutFactor(GetZoomOutFactor() + 1 * Time::GetDeltaTimeUnscaled());
            GetTransform()->scale = Vector3d::one * GetZoomOutFactor();
        }
        if (D2DInput::isKeyDown(KeyCode::E))
        {
            SetZoomOutFactor(GetZoomOutFactor() - 1 * Time::GetDeltaTimeUnscaled());
            GetTransform()->scale = Vector3d::one * GetZoomOutFactor();
        }

        constexpr double speed[5] = { 500,1000,2000,4000,8000 };
        GetTransform()->SetWorldPosition(GetTransform()->GetWorldPosition() + dir * speed[2] * Time::GetDeltaTimeUnscaled());
    }
    else
    {
        if (!lockState)
        {
            recoveryTimer.duration = 3;
            optimalPosition = GetOptimalPosition();
            if (optimalPosition != lastCamPoint)
                if (!recoveryTimer.isActive)
                {
                    if ((optimalPosition - lastCamPoint).Magnitude() > 100)
                    {
                        lastCamPoint = GetTransform()->GetWorldPosition();
                        recoveryTimer.Start();
                    }
                    else
                    {
                        GetTransform()->SetWorldPosition(optimalPosition);
                    }
                }
            lastCamPoint = GetTransform()->GetWorldPosition();
        }
        //DebugObject::CreatePopUpCircle(this->GetGameObject());

        /// 카메라 레일을 따라가지 않는 경우 사용할 코드
        //GetTransform()->SetWorldPosition(Player::GetPlayerPos() + Vector2d(0,200));
    }
    for (auto& each : threatsLegacy)
    {
        each.second += Time::GetDeltaTime();
    }
    vector<Threat*> deleteList;
    for (auto each : threatsLegacy)
    {
        if (each.second > 5)
            deleteList.push_back(each.first);
    }
    for (auto each : deleteList)
    {
        threatsLegacy.erase(each);
        ThreatPool::GetInstance()->Retrieve(each);
    }
}
void GSCamera::OnCollisionStay2D(const Collision2D& collision)
{
    auto threat = collision.m_OtherCollider->GetGameObject()->GetComponent<Threat>();
    if (threat)
    {
        threatsLegacy[threat] = 0;
    }
}

void GSCamera::SetCameraPosToRail()
{
    if (!Player::GetInstance() || (!CameraRail::nowRail))
    {
        return;
    }
    Vector2d playerPos = Player::GetPlayerPos();
    Vector2d moveVector = Vector2d();
    CameraRail* rail = CameraRail::nowRail;

    Vector2d camPos1 = GetTransform()->GetWorldPosition();
    if (rail->railVector.x >= 0)
    {
        if (camPos1.x + rail->cameraProgressThreshold.x <= playerPos.x)
        {
            rail->progress = (camPos1 - rail->startVector).Magnitude() / rail->railVector.Magnitude();
            if (rail->progress >= 1)
            {
                CameraRail::nowRail = rail->nextRail;
                return;
            }
            moveVector.x = playerPos.x - (camPos1.x + rail->cameraProgressThreshold.x);
            moveVector.y = moveVector.x * rail->railVector.y / rail->railVector.x;
            this->GetTransform()->SetWorldPosition(rail->startVector + rail->progress * rail->railVector + moveVector);
            //DebugObject::CreatePopUpCircle(playerPos, 30, 0.5, D2D1::ColorF::Yellow);
        }

        Vector2d camPos2 = GetTransform()->GetWorldPosition();
        if (rail->railVector.y >= 0)
        {
            if (camPos2.y + rail->cameraProgressThreshold.y <= playerPos.y)
            {
                rail->progress = (camPos2 - rail->startVector).Magnitude() / rail->railVector.Magnitude();
                if (rail->progress >= 1)
                {
                    CameraRail::nowRail = rail->nextRail;
                    return;
                }
                moveVector.y = playerPos.y - (camPos2.y + rail->cameraProgressThreshold.y);
                moveVector.x = moveVector.y * rail->railVector.x / rail->railVector.y;
                this->GetTransform()->SetWorldPosition(rail->startVector + rail->progress * rail->railVector + moveVector);
                //DebugObject::CreatePopUpCircle(playerPos, 30, 0.5, D2D1::ColorF::Yellow);
            }
        }
        else
        {
            if (camPos2.y - rail->cameraProgressThreshold.y >= playerPos.y)
            {
                rail->progress = (rail->startVector - camPos2).Magnitude() / rail->railVector.Magnitude();
                if (rail->progress >= 1)
                {
                    CameraRail::nowRail = rail->nextRail;
                    return;
                }
                moveVector.y = playerPos.y - (camPos2.y - rail->cameraProgressThreshold.y);
                moveVector.x = moveVector.y * rail->railVector.x / rail->railVector.y;
                this->GetTransform()->SetWorldPosition(rail->startVector + rail->progress * rail->railVector + moveVector);
                //DebugObject::CreatePopUpCircle(playerPos, 30, 0.5, D2D1::ColorF::Yellow);
            }
        }
    }
    else
    {
        if (camPos1.x - rail->cameraProgressThreshold.x >= playerPos.x)
        {
            rail->progress = (rail->startVector - camPos1).Magnitude() / rail->railVector.Magnitude();
            if (rail->progress >= 1)
            {
                CameraRail::nowRail = rail->nextRail;
                return;
            }
            moveVector.x = playerPos.x - (camPos1.x - rail->cameraProgressThreshold.x);
            moveVector.y = moveVector.x * rail->railVector.y / rail->railVector.x;
            this->GetTransform()->SetWorldPosition(rail->startVector + rail->progress * rail->railVector + moveVector);
            //DebugObject::CreatePopUpCircle(playerPos, 30, 0.5, D2D1::ColorF::Yellow);
        }

        Vector2d camPos2 = GetTransform()->GetWorldPosition();
        if (rail->railVector.y >= 0)
        {
            if (camPos2.y + rail->cameraProgressThreshold.y <= playerPos.y)
            {
                rail->progress = (camPos2 - rail->startVector).Magnitude() / rail->railVector.Magnitude();
                if (rail->progress >= 1)
                {
                    CameraRail::nowRail = rail->nextRail;
                    return;
                }
                moveVector.y = playerPos.y - (camPos2.y + rail->cameraProgressThreshold.y);
                moveVector.x = moveVector.y * rail->railVector.x / rail->railVector.y;
                this->GetTransform()->SetWorldPosition(rail->startVector + rail->progress * rail->railVector + moveVector);
                //DebugObject::CreatePopUpCircle(playerPos, 30, 0.5, D2D1::ColorF::Yellow);
            }
        }
        else
        {
            if (camPos2.y - rail->cameraProgressThreshold.y >= playerPos.y)
            {
                rail->progress = (rail->startVector - camPos2).Magnitude() / rail->railVector.Magnitude();
                if (rail->progress >= 1)
                {
                    CameraRail::nowRail = rail->nextRail;
                    return;
                }
                moveVector.y = playerPos.y - (camPos2.y - rail->cameraProgressThreshold.y);
                moveVector.x = moveVector.y * rail->railVector.x / rail->railVector.y;
                this->GetTransform()->SetWorldPosition(rail->startVector + rail->progress * rail->railVector + moveVector);

                //DebugObject::CreatePopUpCircle(playerPos, 30, 0.5, D2D1::ColorF::Yellow);
            }
        }
    }
}

void GSCamera::LockPosition(const Vector3d& lockPos)
{
    if (lockState)
        return;
    lockState = true;
    recoveryTimer.duration = 1;
    recoveryTimer.Start();
    lastCamPoint = GetTransform()->GetWorldPosition();
    lockPosition = lockPos;
}

void GSCamera::UnLockPosition()
{
    if (!lockState)
        return;
    lockState = false;
    recoveryTimer.Start();
    lastCamPoint = GetTransform()->GetWorldPosition();
}

Vector2d GSCamera::GetOptimalPosition()
{
    if (!Player::GetInstance() || (!CameraRail::nowRail))
    {
        return optimalPosition;
    }
    Vector2d playerPos = Player::GetPlayerPos();
    Vector2d moveVector = Vector2d();
    CameraRail* rail = CameraRail::nowRail;

    Vector2d camPos1 = optimalPosition;
    Vector2d vectorContainer = optimalPosition;
    if (rail->railVector.x >= 0)
    {
        if (camPos1.x + rail->cameraProgressThreshold.x <= playerPos.x)
        {
            rail->progress = (camPos1 - rail->startVector).Magnitude() / rail->railVector.Magnitude();
            if (rail->progress >= 1)
            {
                CameraRail::nowRail = rail->nextRail;
                return GetOptimalPosition();
            }
            moveVector.x = playerPos.x - (camPos1.x + rail->cameraProgressThreshold.x);
            moveVector.y = moveVector.x * rail->railVector.y / rail->railVector.x;
            //DebugObject::CreatePopUpCircle(playerPos, 30, 0.5, D2D1::ColorF::Yellow);
            vectorContainer = rail->startVector + rail->progress * rail->railVector + moveVector;
        }

        if (rail->railVector.y >= 0)
        {
            if (vectorContainer.y + rail->cameraProgressThreshold.y <= playerPos.y)
            {
                rail->progress = (vectorContainer - rail->startVector).Magnitude() / rail->railVector.Magnitude();
                if (rail->progress >= 1)
                {
                    CameraRail::nowRail = rail->nextRail;
                    return GetOptimalPosition();
                }
                moveVector.y = playerPos.y - (vectorContainer.y + rail->cameraProgressThreshold.y);
                moveVector.x = moveVector.y * rail->railVector.x / rail->railVector.y;
                return (rail->startVector + rail->progress * rail->railVector + moveVector);
            }
            return vectorContainer;
        }
        else
        {
            if (vectorContainer.y - rail->cameraProgressThreshold.y >= playerPos.y)
            {
                rail->progress = (rail->startVector - vectorContainer).Magnitude() / rail->railVector.Magnitude();
                if (rail->progress >= 1)
                {
                    CameraRail::nowRail = rail->nextRail;
                    return GetOptimalPosition();
                }
                moveVector.y = playerPos.y - (vectorContainer.y - rail->cameraProgressThreshold.y);
                moveVector.x = moveVector.y * rail->railVector.x / rail->railVector.y;
                return (rail->startVector + rail->progress * rail->railVector + moveVector);
            }
            return vectorContainer;
        }
    }
    else
    {
        if (camPos1.x - rail->cameraProgressThreshold.x >= playerPos.x)
        {
            rail->progress = (rail->startVector - camPos1).Magnitude() / rail->railVector.Magnitude();
            if (rail->progress >= 1)
            {
                CameraRail::nowRail = rail->nextRail;
                return GetOptimalPosition();
            }
            moveVector.x = playerPos.x - (camPos1.x - rail->cameraProgressThreshold.x);
            moveVector.y = moveVector.x * rail->railVector.y / rail->railVector.x;
            vectorContainer = rail->startVector + rail->progress * rail->railVector + moveVector;
        }

        if (rail->railVector.y >= 0)
        {
            if (vectorContainer.y + rail->cameraProgressThreshold.y <= playerPos.y)
            {
                rail->progress = (vectorContainer - rail->startVector).Magnitude() / rail->railVector.Magnitude();
                if (rail->progress >= 1)
                {
                    CameraRail::nowRail = rail->nextRail;
                    return GetOptimalPosition();
                }
                moveVector.y = playerPos.y - (vectorContainer.y + rail->cameraProgressThreshold.y);
                moveVector.x = moveVector.y * rail->railVector.x / rail->railVector.y;
                return (rail->startVector + rail->progress * rail->railVector + moveVector);
            }
            return vectorContainer;
        }
        else
        {
            if (vectorContainer.y - rail->cameraProgressThreshold.y >= playerPos.y)
            {
                rail->progress = (rail->startVector - vectorContainer).Magnitude() / rail->railVector.Magnitude();
                if (rail->progress >= 1)
                {
                    CameraRail::nowRail = rail->nextRail;
                    return GetOptimalPosition();
                }
                moveVector.y = playerPos.y - (vectorContainer.y - rail->cameraProgressThreshold.y);
                moveVector.x = moveVector.y * rail->railVector.x / rail->railVector.y;
                return (rail->startVector + rail->progress * rail->railVector + moveVector);
            }
            return vectorContainer;
        }
    }
}
