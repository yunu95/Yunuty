#include "Guardian_shooter.h"
#include "Bullet.h"
#include "ShooterOneShot.h"
#include "Shooter1.h"
#include "Shooter2.h"
#include "Shooter3.h"
#include "Bomber.h"
#include "Player.h"
#include "ProjectilePool.h"
#include "GlobalReferences.h"
#include "DebugObject.h"

Bullet::Bullet()
{
	projType = ProjectileType::BULLET;
	destroyAnimTimer.duration = 0.5;
	destroyAnimTimer.onExpiration = [this]()
	{
		isTimerOn = false;
		ProjectilePool::GetInstance()->Retrieve(this);
	};
}

void Bullet::Initalize(D2DAnimatedSprite* sprite)
{
	animatedSprite = sprite;
	animatedSprite->SetIsRepeating(false);
}

void Bullet::Destroy()
{
	if (!isTimerOn)
	{
		destroyAnimTimer.Start();
		if (isPlayerProj)
			animatedSprite->LoadAnimationFromFile(L"sprites/projectile/bullet/player/destroy");
		else
			animatedSprite->LoadAnimationFromFile(L"sprites/projectile/bullet/enemy/destroy");
	}
	isTimerOn = true;
}

void Bullet::Update()
{
	if (isTimerOn)
	{
		destroyAnimTimer.Update();
	}
	else
	{
		GetTransform()->SetWorldPosition(GetTransform()->GetWorldPosition() + speed * Time::GetDeltaTime());
		GetTransform()->SetWorldRotation(Vector3d(0, 0, Vector2d(speed).GetAngleDegree()));
	}
}

void Bullet::OnCollisionEnter2D(const Collision2D& collision)
{
	if (isTimerOn)
		return;

	auto ground = collision.m_OtherCollider->GetGameObject()->GetComponent<Ground>();
	if (ground != nullptr && !ground->isPenetrable)
	{
		Destroy();
		return;
	}

	if (isPlayerProj == false)
	{
		auto player = collision.m_OtherCollider->GetGameObject()->GetComponent<Player>();
		if (player != nullptr)
		{
			player->Damage();
			Destroy();
			return;
		}
	}
	else
	{
		auto missle = collision.m_OtherCollider->GetGameObject()->GetComponent<Projectile>();
		if (missle != nullptr)
		{
			if (missle->projType == ProjectileType::MISSILE)
			{
				Destroy();
				return;
			}
		}
		auto enemy = collision.m_OtherCollider->GetGameObject()->GetComponent<Threat>();
		if (enemy != nullptr)
		{
			if (enemy->hp > 0)
			{
				enemy->Damage(damage);
				Destroy();
				return;
			}
		}
	}
}


void Bullet::OnCollisionExit2D(const Collision2D& collision)
{
	if (collision.m_OtherCollider == GlobalReference::cameraRectCollider)
	{
		ProjectilePool::GetInstance()->Retrieve(this);
	}
}

void Bullet::CreateBullet(Vector3d speed)
{
	if (ProjectilePool::GetInstance() == nullptr)
	{
		GameObject* projectilePool = Scene::getCurrentScene()->AddGameObject();
		projectilePool->AddComponent<ProjectilePool>();
	}
	auto bullet = Scene::getCurrentScene()->AddGameObject();
	bullet->AddComponent<BoxCollider2D>()->SetHeight(14);
	bullet->GetComponent<BoxCollider2D>()->SetWidth(40);
	DebugObject::CreateColliderImage(bullet->GetComponent<BoxCollider2D>());
	bullet->AddComponent<Bullet>()->speed = speed;
	auto anim = bullet->AddComponent<D2DAnimatedSprite>();
	anim->SetWidth(64);
	anim->SetHeight(32);
	bullet->GetComponent<Bullet>()->Initalize(anim);
	bullet->SetParent(MapTool::GetInstance()->GetProjectileLayer());
	ProjectilePool::GetInstance()->Retrieve(bullet->GetComponent<Bullet>());
}
