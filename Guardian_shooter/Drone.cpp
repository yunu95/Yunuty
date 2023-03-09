#include "Drone.h"
#include "Player.h"
#include "ProjectilePool.h"

Drone* Drone::instance = nullptr;

Drone::Drone()
{

}

void Drone::Start()
{
	instance = this;
	damage = 2;
	attackCoolDownTimer.duration = 0.1;
}

Drone* Drone::GetInstance()
{
	return this->instance;
}

void Drone::Initialize(D2DAnimatedSprite* animatedSprite)
{
	this->animatedSprite = animatedSprite;
	animatedSprite->LoadAnimationFromFile(L"sprites/drone/idle");
}

GameObject* Drone::CreateDrone(Vector3d pos)
{
	auto drone = Scene::getCurrentScene()->AddGameObject();

	drone->AddComponent<Drone>();

	drone->GetTransform()->SetWorldPosition(pos);
	drone->SetSelfActive(false);
	auto anim = drone->AddComponent<D2DAnimatedSprite>();
	anim->SetHeight(64);
	anim->SetWidth(220);
	drone->GetComponent<Drone>()->Initialize(anim);

	return drone;
}

void Drone::Update()
{
	attackCoolDownTimer.Update();
	dronePos = GetTransform()->GetWorldPosition();

	if (D2DInput::isKeyDown(KeyCode::Z))
	{
		justAttack = true;
		attackJustchanged = true;
	}

	if (attackJustchanged)
	{
		//SoundSystem::PlaySoundfile("sounds/player/Player_droneshoot.wav");
		animatedSprite->LoadAnimationFromFile(L"sprites/drone/attack");
		attackJustchanged = false;
	}
	else
	{
		animatedSprite->LoadAnimationFromFile(L"sprites/drone/idle");
	}

	if (Player::GetPlayerDirection() == 1)
	{
		this->GetTransform()->scale.x = 1;
		GetTransform()->position = Vector3d(-100, 80, 0);
	}
	else
	{
		this->GetTransform()->scale.x = -1;
		GetTransform()->position = Vector3d(100, 80, 0);
	}

	if (justAttack)
	{
		if (!attackCoolDownTimer.isActive)
		{
			if (Player::GetPlayerDirection() == 1)
			{
				ProjectilePool::GetInstance()->
					SummonProjectile(GetTransform()->GetWorldPosition() + Vector3d(65, -15, 0), Vector3d(1600, 0, 0), ProjectileType::BULLET, damage, true);
			}
			else if (Player::GetPlayerDirection() == -1)
			{
				ProjectilePool::GetInstance()->
					SummonProjectile(GetTransform()->GetWorldPosition() + Vector3d(-65, -15, 0), Vector3d(-1600, 0, 0), ProjectileType::BULLET, damage, true);
			}
			attackCoolDownTimer.Start();
		}
		justAttack = false;
	}

}
