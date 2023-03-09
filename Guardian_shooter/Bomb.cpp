#include "Bomb.h"
#include "Bomber.h"
#include "Player.h"
#include "Ground.h"
#include "ProjectilePool.h"
#include "GlobalReferences.h"
#include "DebugObject.h"
#include "Guardian_shooter.h"

Bomb::Bomb()
{
    projType = ProjectileType::BOMB;
    destroyAnimTimer.duration = 0.5;
    destroyAnimTimer.onExpiration = [this]()
    {
        isTimerOn = false;
        isExplosionTimerEnd = false;
        ProjectilePool::GetInstance()->Retrieve(this);
    };
    explosionDamageTimer.duration = 0.35;
    explosionDamageTimer.onExpiration = [this]()
    {
        isExplosionTimerEnd = true;
    };
}

void Bomb::Initalize(D2DAnimatedSprite* sprite)
{
    animatedSprite = sprite;
    animatedSprite->SetIsRepeating(false);
}

void Bomb::Destroy()
{
    if (!isTimerOn)
    {
        destroyAnimTimer.Start();
        explosionDamageTimer.Start();
        SoundSystem::PlaySoundfile("sounds/enemy/Enemy_grenade_explode.wav");
        animatedSprite->LoadAnimationFromFile(L"sprites/projectile/bomb/destroy");
        animatedSprite->Play();
    }
    isTimerOn = true;
}

void Bomb::Update()
{
    if (isTimerOn)
    {
        destroyAnimTimer.Update();
        explosionDamageTimer.Update();
    }
    else
    {
        speed.y += -gravity * 10 * Time::GetDeltaTime();
        GetTransform()->SetWorldPosition(GetTransform()->GetWorldPosition() + speed * Time::GetDeltaTime());
        GetTransform()->SetWorldRotation(Vector3d(0, 0, Vector2d(speed).GetAngleDegree()));
    }
}

void Bomb::OnCollisionEnter2D(const Collision2D& collision)
{
    if (isExplosionTimerEnd)
        return;
    auto ground = collision.m_OtherCollider->GetGameObject()->GetComponent<Ground>();
    if (ground != nullptr && !ground->isPenetrable)
    {
        Destroy();
        return;
    }

    if (isPlayerProj)
    {
        auto enemy = collision.m_OtherCollider->GetGameObject()->GetComponent<Threat>();
        if (enemy != nullptr)
        {
            enemy->Damage(damage);
            Destroy();
        }
    }
    else
    {
        auto player = collision.m_OtherCollider->GetGameObject()->GetComponent<Player>();
        if (player != nullptr)
        {
            player->Damage();
            Destroy();
            return;
        }
    }
}


void Bomb::OnCollisionExit2D(const Collision2D& collision)
{
    if (collision.m_OtherCollider == GlobalReference::cameraRectCollider)
    {
        if (GetTransform()->GetWorldPosition().y >= GlobalReference::cameraRectCollider->GetTransform()->GetWorldPosition().y)
            return;
        ProjectilePool::GetInstance()->Retrieve(this);
    }
}

void Bomb::CreateBomb(Vector3d speed)
{
    if (ProjectilePool::GetInstance() == nullptr)
    {
        GameObject* projectilePool = Scene::getCurrentScene()->AddGameObject();
        projectilePool->AddComponent<ProjectilePool>();
    }
    auto bomb = Scene::getCurrentScene()->AddGameObject();
    bomb->AddComponent<BoxCollider2D>()->SetHeight(24);
    bomb->GetComponent<BoxCollider2D>()->SetWidth(56);
    DebugObject::CreateColliderImage(bomb->GetComponent<BoxCollider2D>());
    bomb->AddComponent<Bomb>()->speed = speed;
    auto anim = bomb->AddComponent<D2DAnimatedSprite>();
    anim->SetWidth(128);
    anim->SetHeight(64);
    bomb->GetComponent<Bomb>()->Initalize(anim);
    bomb->SetParent(MapTool::GetInstance()->GetProjectileLayer());
    ProjectilePool::GetInstance()->Retrieve(bomb->GetComponent<Bomb>());
}
