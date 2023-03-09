#include "Missile.h"
#include "Player.h"
#include "Boss.h"
#include "Ground.h"
#include "ProjectilePool.h"
#include "GlobalReferences.h"
#include "DebugObject.h"
#include "Guardian_shooter.h"

Missile::Missile()
{
	projType = ProjectileType::MISSILE;
	destroyAnimTimer.duration = 0.3;
	destroyAnimTimer.onExpiration = [this]()
	{
		isTimerOn = false;
		ProjectilePool::GetInstance()->Retrieve(this);
	};
}

void Missile::Initalize(D2DAnimatedSprite* sprite)
{
	animatedSprite = sprite;
	animatedSprite->SetIsRepeating(false);
}

void Missile::Destroy()
{
	if (!isTimerOn)
	{
		destroyAnimTimer.Start();
		SoundSystem::PlaySoundfile("sounds/boss/Boss_pt2laucherexplode.mp3");
		animatedSprite->LoadAnimationFromFile(L"sprites/projectile/bomb/destroy");
	}
	isTimerOn = true;
}

void Missile::Update()
{
	if (isTimerOn)
	{
		destroyAnimTimer.Update();
	}
	else
	{
		Vector3d playerPos = Player::GetPlayerPos();
		if (isPlayerProj)
			speed = Boss::GetInstance()->GetBossPos() - playerPos;
		else
			speed = Vector3d(playerPos.x - GetTransform()->GetWorldPosition().x, playerPos.y - GetTransform()->GetWorldPosition().y + 55, 0);
		GetTransform()->SetWorldPosition(GetTransform()->GetWorldPosition() + speed * 1.3 * Time::GetDeltaTime());
		GetTransform()->SetWorldRotation(Vector3d(0, 0, Vector2d(speed).GetAngleDegree()));
	}
}

void Missile::OnCollisionEnter2D(const Collision2D& collision)
{
	if (isTimerOn)
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
			return;
		}
	}
	else
	{
		auto playerBullet = collision.m_OtherCollider->GetGameObject()->GetComponent<Projectile>();
		if (playerBullet != nullptr)
		{
			if (playerBullet->isPlayerProj)
			{
				Player::GetInstance()->SetGauge(1);
				Destroy();
				return;
			}
		}

		auto player = collision.m_OtherCollider->GetGameObject()->GetComponent<Player>();
		if (player != nullptr)
		{
			player->Damage();
			Destroy();
			return;
		}
	}
}

void Missile::OnCollisionExit2D(const Collision2D& collision)
{
	if (collision.m_OtherCollider == GlobalReference::cameraRectCollider)
	{
		ProjectilePool::GetInstance()->Retrieve(this);
	}
}

void Missile::CreateMissile(Vector3d speed)
{
	if (ProjectilePool::GetInstance() == nullptr)
	{
		GameObject* projectilePool = Scene::getCurrentScene()->AddGameObject();
		projectilePool->AddComponent<ProjectilePool>();
	}
	auto missile = Scene::getCurrentScene()->AddGameObject();
	missile->AddComponent<BoxCollider2D>()->SetHeight(45);
	missile->GetComponent<BoxCollider2D>()->SetWidth(60);
	DebugObject::CreateColliderImage(missile->GetComponent<BoxCollider2D>());
	missile->AddComponent<Missile>()->speed = speed;
	auto anim = missile->AddComponent<D2DAnimatedSprite>();
	anim->SetWidth(128);
	anim->SetHeight(64);
	missile->GetComponent<Missile>()->Initalize(anim);
	ProjectilePool::GetInstance()->Retrieve(missile->GetComponent<Missile>());
}
