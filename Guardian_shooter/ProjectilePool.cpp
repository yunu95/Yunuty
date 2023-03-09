#include "Guardian_shooter.h"
#include "ProjectilePool.h"
#include "Bullet.h"
#include "Bomb.h"
#include "Missile.h"
#include "BossBullet.h"

ProjectilePool* ProjectilePool::instance = nullptr;

void ProjectilePool::RetriveAll()
{
    //for (auto each : projectiles)
    //    Retrieve(each.second);
}

Projectile* ProjectilePool::SummonProjectile(Vector3d position, Vector3d speed, ProjectileType projType, double damage, bool isPlayerProj)
{
    auto iter = projectiles.find(projType);
    if (iter == projectiles.end())
    {
        switch (projType)
        {
        case ProjectileType::BULLET:
            Bullet::CreateBullet(speed);
            break;
        case ProjectileType::BOMB:
            Bomb::CreateBomb(speed);
            break;
        case ProjectileType::MISSILE:
            Missile::CreateMissile(speed);
            break;
        case ProjectileType::BOSSBULLET:
            BossBullet::CreateBossBullet(speed);
            break;
        default:
            break;
        }
        iter = projectiles.find(projType);
    }
    auto ret = iter->second;
    ret->damage = damage;
    ret->isPlayerProj = isPlayerProj;
    if (ret->isPlayerProj)
        ret->GetGameObject()->SetParent(MapTool::GetInstance()->GetPlayerProjectileLayer());
    else
        ret->GetGameObject()->SetParent(MapTool::GetInstance()->GetProjectileLayer());

    ret->GetTransform()->SetWorldPosition(position);
    ret->speed = speed;
    switch (projType)
    {
    case ProjectileType::BULLET:
    case ProjectileType::BOSSBULLET:
        if (isPlayerProj)
            ret->animatedSprite->LoadAnimationFromFile(L"sprites/projectile/bullet/player/forward");
        else
            ret->animatedSprite->LoadAnimationFromFile(L"sprites/projectile/bullet/enemy/forward");
        break;
    case ProjectileType::BOMB:
        ret->animatedSprite->LoadAnimationFromFile(L"sprites/projectile/bomb/forward");
        break;
    case ProjectileType::MISSILE:
        ret->animatedSprite->LoadAnimationFromFile(L"sprites/projectile/bomb/forward");
        break;
    default:
        break;
    }
    ret->GetGameObject()->SetSelfActive(true);
    projectiles.erase(iter);
    return ret;
}


void ProjectilePool::Retrieve(Projectile* projectile)
{
    projectiles.insert(std::make_pair(projectile->projType, projectile));
    projectile->GetGameObject()->SetSelfActive(false);
}