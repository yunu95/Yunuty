#include "Shooter3.h"
#include "DebugObject.h"
#include "Player.h"
#include "Ground.h"
#include "ShootingLaser.h"
#include "ThreatPool.h"
#include "DamageEffect.h"


void Shooter3::Initialize(D2DAnimatedSprite* animatedSprite)
{
	this->anim = animatedSprite;
}

GameObject* Shooter3::CreateShooter3(Vector3d pos)
{
	// ThreatPool 없으면 생성.
	if (ThreatPool::GetInstance() == nullptr)
	{
		GameObject* threatPool = Scene::getCurrentScene()->AddGameObject();
		threatPool->AddComponent<ThreatPool>();
	}

	auto shooter3 = Scene::getCurrentScene()->AddGameObject();

	shooter3->AddComponent<BoxCollider2D>();
	shooter3->GetComponent<BoxCollider2D>()->SetWidth(60);
	shooter3->GetComponent<BoxCollider2D>()->SetHeight(160);

	DebugObject::CreateColliderImage(shooter3->GetComponent<BoxCollider2D>());

	shooter3->GetTransform()->SetWorldPosition(pos);

	auto shooterComponent = shooter3->AddComponent<Shooter3>();

	GameObject* shooterImage = Scene::getCurrentScene()->AddGameObject(shooter3);
	auto anim = shooterImage->AddComponent<D2DAnimatedSprite>();
	shooterImage->GetTransform()->position = Vector3d(0, 40, 0);
	anim->SetHeight(256);
	anim->SetWidth(256);
	shooter3->GetComponent<Shooter3>()->Initialize(anim);
	GameObject* laser = ShootingLaser::CreateLaser(Vector3d::zero);
	laser->SetParent(shooter3);
	laser->GetTransform()->position = Vector3d(1250, 5, 0);
	shooter3->GetComponent<Shooter3>()->laser = laser->GetComponent<ShootingLaser>();

	GameObject* effect = DamageEffect::CreateDamageEffect(Vector3d::zero);
	effect->SetParent(shooter3);
	effect->SetSelfActive(false);
	shooterComponent->effect = effect->GetComponent<DamageEffect>();
	
	return shooter3;
}

void Shooter3::Start()
{
	// 초기화 구문
	hp = 4;
	damage = 1.f;
	gravity = 100;
	bulletCnt = 0;
	canAttack = false;

	justLanded = false;
	justDamaged = false;
	justFallen = false;
	stateJustChanged = true;
	isSummoned = false;

	state = Shooter3State::FALLING;

	traceTimer.duration = 0.9;		// 이동으로 전환되는 시간

	chargeStartTimer.duration = 2;

	attackStartTimer.duration = 0.8;
	attackStartTimer.onExpiration = [this]()
	{
		laser->GetGameObject()->SetSelfActive(true);
	};

	laserLastTimer.duration = 1.0;
	laserLastTimer.onExpiration = [this]()
	{
		laser->GetGameObject()->SetSelfActive(false);
	};

	animTimer.duration = 0.25;
	animTimer.onExpiration = [this]()
	{
		ThreatPool::GetInstance()->Retrieve(this);
	};

	damageEffect.duration = 0.7;
	damageEffect.onExpiration = [this]()
	{
		effect->GetGameObject()->SetSelfActive(false);
	};

}

void Shooter3::changeState(Shooter3State changedState)
{
	state = changedState;
	stateJustChanged = true;
}

void Shooter3::Update()
{
	// 업데이트 구문
	traceTimer.Update();
	chargeStartTimer.Update();
	attackStartTimer.Update();
	laserLastTimer.Update();
	damageEffect.Update();

	// 적군 위치
	GetTransform()->SetWorldPosition(
		GetTransform()->GetWorldPosition()
		+ speed * Time::GetDeltaTime());

	// 추적하고자 하는 플레이어의 위치
	Vector3d playerPos = Player::GetPlayerPos();

	// 수평 공격 가능 범위
	if (abs(playerPos.x - GetTransform()->GetWorldPosition().x) < 1600 &&
		abs(playerPos.y - GetTransform()->GetWorldPosition().y) < 350)
	{
		canAttack = true;
	}
	else
		canAttack = false;


	/// <summary>
/// 객체의 상태가 다른 상태로 바뀔 수 있는지를 먼저 따진다.
/// </summary>\

	switch (state)
	{
	case Shooter3State::IDLE:
		if (canAttack == false && traceTimer.isActive == false)
		{
			changeState(Shooter3State::WALKING);
		}
		if (canAttack == true && !chargeStartTimer.isActive)
		{
			changeState(Shooter3State::CHARGE);
		}
		break;

	case Shooter3State::FALLING:
		if (justLanded == true)
		{
			changeState(Shooter3State::IDLE);
			justLanded = false;
		}
		break;

	case Shooter3State::WALKING:
		if (canAttack == true)
		{
			changeState(Shooter3State::IDLE);
		}
		if (justFallen == true)
		{
			changeState(Shooter3State::FALLING);
			justFallen = false;
		}
		break;

	case Shooter3State::CHARGE:
		if (!attackStartTimer.isActive)
		{
			changeState(Shooter3State::ATTACK);
		}
		break;

	case Shooter3State::ATTACK:
		if (!laserLastTimer.isActive)
		{
			changeState(Shooter3State::IDLE);
		}
		break;

	case Shooter3State::DEATH:
		break;
	default:
		break;
	}
	/// <summary>
	/// 각 상태에 따른 행동은 여기서 진행합니다.
	/// </summary>
	switch (state)
	{
	case Shooter3State::IDLE:
		if (stateJustChanged)
		{
			anim->LoadAnimationFromFile(L"sprites/enemy/shooter3/idle");
			speed.x = 0;
			speed.y = 0;
			traceTimer.Start();
			chargeStartTimer.Start();
			if (playerPos.x > GetTransform()->GetWorldPosition().x)
			{
				GetTransform()->scale.x = 1;
			}
			else if (playerPos.x < GetTransform()->GetWorldPosition().x)
			{
				GetTransform()->scale.x = -1;
			}
		}
		break;

	case Shooter3State::CHARGE:
		if (stateJustChanged)
		{
			// 여기에 준비 애니메이션 넣기
			anim->LoadAnimationFromFile(L"sprites/enemy/shooter3/charge");
			SoundSystem::PlaySoundfile("sounds/enemy/Enemy_Lazercharge.wav");
			attackStartTimer.Start();
		}
		break;

	case Shooter3State::ATTACK:
		if (stateJustChanged)
		{
			// 여기에 준비 애니메이션 넣기
			speed.x = 0;
			speed.y = 0;
			anim->LoadAnimationFromFile(L"sprites/enemy/shooter3/attack");
			SoundSystem::PlaySoundfile("sounds/enemy/Enemy_Lazershoot.wav");
			laserLastTimer.Start();
		}
		break;

	case Shooter3State::DEATH:
		animTimer.Update();
		if (stateJustChanged)
		{
			speed.x = 0;
			speed.y = 0;
			anim->LoadAnimationFromFile(L"sprites/enemy/shooter3/death");
			SoundSystem::PlaySoundfile("sounds/enemy/Enemy_dyingexplode.wav");
			animTimer.Start();
		}
		break;

	case Shooter3State::FALLING:
		speed += Vector3d(0, -gravity, 0) * 10 * Time::GetDeltaTime();

		if (stateJustChanged)
		{
			anim->LoadAnimationFromFile(L"sprites/enemy/shooter3/fall");
			speed.x = 0;
		}
		break;

	case Shooter3State::WALKING:
		if (stateJustChanged)
		{
			if (playerPos.x > this->GetTransform()->GetWorldPosition().x)
				speed.x = 300;
			if (playerPos.x < this->GetTransform()->GetWorldPosition().x)
				speed.x = -300;
			anim->LoadAnimationFromFile(L"sprites/enemy/shooter3/move");
			speed.y = 0;
			traceTimer.Start();
		}
		break;

	default:
		break;
	}

	stateJustChanged = false;
}


void Shooter3::OnCollisionEnter2D(const Collision2D& collision)
{
	auto box = collision.m_OtherCollider->GetGameObject()->GetComponent<Ground>();
	if (box != nullptr)
	{
		justLanded = true;
	}
}

void Shooter3::OnCollisionExit2D(const Collision2D& collision)
{
	auto box = collision.m_OtherCollider->GetGameObject()->GetComponent<Ground>();
	if (box != nullptr)
	{
		justFallen = true;
	}
}

void Shooter3::Damage(double damage)
{
	damageEffect.Start();
	Threat::Damage(damage);
	effect->GetGameObject()->SetSelfActive(true);
}

void Shooter3::OnDeath()
{
	changeState(Shooter3State::DEATH);
}
