#include "Shooter1.h"
#include "DebugObject.h"
#include "Player.h"
#include "Ground.h"
#include "ProjectilePool.h"
#include "ThreatPool.h"
#include "DamageEffect.h"


void Shooter1::Initialize(D2DAnimatedSprite* animatedSprite)
{
	anim = animatedSprite;
}

GameObject* Shooter1::CreateShooter1(Vector3d pos)
{
	// ThreatPool 없으면 생성.
	if (ThreatPool::GetInstance() == nullptr)
	{
		GameObject* threatPool = Scene::getCurrentScene()->AddGameObject();
		threatPool->AddComponent<ThreatPool>();
	}

	auto shooter1 = Scene::getCurrentScene()->AddGameObject();
	
	shooter1->AddComponent<BoxCollider2D>();
	shooter1->GetComponent<BoxCollider2D>()->SetWidth(60);
	shooter1->GetComponent<BoxCollider2D>()->SetHeight(150);

	auto shooterComponent = shooter1->AddComponent<Shooter1>();
	shooterComponent->hp = 3;
	shooter1->GetTransform()->SetWorldPosition(pos);

	GameObject* shooterImage = Scene::getCurrentScene()->AddGameObject(shooter1);
	auto anim1 = shooterImage->AddComponent<D2DAnimatedSprite>();
	shooterImage->GetTransform()->position = Vector3d(0, 40, 0);
	anim1->SetHeight(256);
	anim1->SetWidth(256);
	shooter1->GetComponent<Shooter1>()->Initialize(anim1);

	GameObject* effect = DamageEffect::CreateDamageEffect(Vector3d::zero);
	effect->SetParent(shooter1);
	effect->SetSelfActive(false);
	shooterComponent->effect = effect->GetComponent<DamageEffect>();

	DebugObject::CreateDebugRectImage(shooter1, 70, 170, D2D1::ColorF::RoyalBlue);

	return shooter1;
}

void Shooter1::Start()
{
	// 초기화 구문
	/*instance = this;*/
	hp = 3;
	damage = 3.f;
	gravity = 100;
	bulletCnt = 0;
	canAttack = false;
	attackHorable = false;
	attackVerable = false;
	justLanded = false;
	justDamaged = false;
	justFallen = false;
	stateJustChanged = true;
	isSummoned = false;
	attackFinished = false;
	
	state = Shooter1State::FALLING;

	traceTimer.duration = 1.0;		// 이동으로 전환되는 시간
	attackDelay.duration = 0.3;
	attackDelay2.duration = 1.5;
	attackDelay2.onExpiration = [this]()
	{
		attackFinished = true;
	};
	interrupted.duration = 0.9;
	animTimer.duration = 0.15;
	animTimer.onExpiration = [this]()
	{
		ThreatPool::GetInstance()->Retrieve(this);
	};
	attackStop.duration = 0.2;
	attackStop.onExpiration = [this]()
	{
		changeState(Shooter1State::WALKING);
	};
	damageEffect.duration = 0.7;
	damageEffect.onExpiration = [this]()
	{
		effect->GetGameObject()->SetSelfActive(false);
	};
}

void Shooter1::changeState(Shooter1State changedState)
{
	state = changedState;
	stateJustChanged = true;
}

void Shooter1::Update()
{
	// 업데이트 구문
	traceTimer.Update();
	attackDelay.Update();
	attackDelay2.Update();
	interrupted.Update();
	attackStop.Update();
	damageEffect.Update();


	// 적군 위치
	GetTransform()->SetWorldPosition(
		GetTransform()->GetWorldPosition()
		+ speed * Time::GetDeltaTime());

	// 추적하고자 하는 플레이어의 위치
	Vector3d playerPos = Player::GetPlayerPos();

	// 수평 공격 가능 범위
	if (abs(playerPos.x - GetTransform()->GetWorldPosition().x) < randHorRange &&
		abs(playerPos.y - GetTransform()->GetWorldPosition().y) < 400
		&& bulletCnt == 0)
	{
		attackHorable = true;
		//attackVerable = false;		// 중복 사격 방지
	}
	else
	{
		attackHorable = false;
	}

	// 수직 공격 가능 범위
	if (abs(playerPos.x - GetTransform()->GetWorldPosition().x) < randVerRange &&
		playerPos.y - GetTransform()->GetWorldPosition().y > 400
		&& bulletCnt == 0)
	{
		attackVerable = true;
		//attackHorable = false;		// 중복 사격 방지
	}
	else
	{
		attackVerable = false;
	}

	if (attackHorable || attackVerable)
		canAttack = true;
	else
		canAttack = false;

	if (state != Shooter1State::ATTACK)			// 점사 도중 상태 바뀔 시 카운트 초기화
		bulletCnt = 0;

	/// <summary>
/// 객체의 상태가 다른 상태로 바뀔 수 있는지를 먼저 따진다.
/// </summary>\

	switch (state)
	{
	case Shooter1State::IDLE:
		if (canAttack == false && traceTimer.isActive == false)
		{
			changeState(Shooter1State::WALKING);
		}
		if (canAttack == true && traceTimer.isActive == false)
		{
			changeState(Shooter1State::ATTACK);
		}
		break;

	case Shooter1State::FALLING:
		if (justLanded == true)
		{
			changeState(Shooter1State::IDLE);
			justLanded = false;
		}
		break;
	case Shooter1State::WALKING:
		if (canAttack == true)
		{
			changeState(Shooter1State::IDLE);
		}
		if (justFallen == true)
		{
			changeState(Shooter1State::FALLING);
			justFallen = false;
		}
		break;
	case Shooter1State::ATTACK:
		if (attackFinished)
		{
			changeState(Shooter1State::IDLE);
		}
		break;
	case Shooter1State::DEATH:
		break;
	default:
		break;
	}
	/// <summary>
	/// 각 상태에 따른 행동은 여기서 진행합니다.
	/// </summary>
	switch (state)
	{
	case Shooter1State::IDLE:
		/// <summary>
		/// 상태가 갓 IDLE 상태로 바뀐 경우에는, stateJustChanged가 참인 상태로 이 조건문을 통과하게 된다.
		/// </summary>
		if (stateJustChanged)
		{
			anim->LoadAnimationFromFile(L"sprites/enemy/shooter1/idle");
			speed.x = 0;
			speed.y = 0;
			traceTimer.Start();
			if (playerPos.x > GetTransform()->GetWorldPosition().x)
			{
				GetTransform()->scale.x = 1;
			}
			if (playerPos.x < GetTransform()->GetWorldPosition().x)
			{
				GetTransform()->scale.x = -1;
			}
			attackHorable = false;
			attackVerable = false;
			//gravity = 0;
		}
		/// <summary>
		/// 상태가 IDLE 상태일 때 매 프레임마다 호출되어야 하는 로직의 경우 여기 아래에 작성된다.
		/// </summary>


		break;
	case Shooter1State::FALLING:
		speed += Vector3d(0, -gravity, 0) * 10 * Time::GetDeltaTime();

		if (stateJustChanged)
		{
			anim->LoadAnimationFromFile(L"sprites/enemy/shooter1/fall");
			speed.x = 0;
		}
		break;
	case Shooter1State::WALKING:
		if (stateJustChanged)
		{
			if (playerPos.x > this->GetTransform()->GetWorldPosition().x)
			{
				speed.x = 300;
				GetTransform()->scale.x = 1;
				
			}
			if (playerPos.x < this->GetTransform()->GetWorldPosition().x)
			{
				speed.x = -300;
				GetTransform()->scale.x = -1;
			}
			anim->LoadAnimationFromFile(L"sprites/enemy/shooter1/move");
			speed.y = 0;
			traceTimer.Start();
			randHorRange = rand() % 350 + 400;
			randVerRange = rand() % 40 + 60;
		}

		break;
	case Shooter1State::ATTACK:
		if (stateJustChanged)
		{
			if (attackHorable)
				anim->LoadAnimationFromFile(L"sprites/enemy/shooter1/attack/forward");
			else if (attackVerable)
				anim->LoadAnimationFromFile(L"sprites/enemy/shooter1/attack/up");
			speed.x = 0;
			speed.y = 0;
			attackFinished = false;
			attackDelay.Start();
			if (playerPos.x > GetTransform()->GetWorldPosition().x)
			{
				isAttacking = true;
				GetTransform()->scale.x = 1;
			}
			if (playerPos.x < GetTransform()->GetWorldPosition().x)
			{
				isAttacking = false;
				GetTransform()->scale.x = -1;
			}
			bulletCnt = 0;
		}

		if (!attackDelay.isActive && !interrupted.isActive && bulletCnt < 3)
		{
			if (attackHorable || bulletCnt >= 1 && !isShootingUp)
			{
				isShootingForward = true;
				if (isAttacking)
				{
					// 오른쪽 사격
					ProjectilePool::GetInstance()->SummonProjectile(GetTransform()->GetWorldPosition() + Vector3d(100, 7, 0)
						, Vector3d(500, 0, 0), ProjectileType::BULLET, damage, false);
				}
				else
				{
					// 왼쪽 사격
					ProjectilePool::GetInstance()->SummonProjectile(GetTransform()->GetWorldPosition() + Vector3d(-100, 7, 0)
						, Vector3d(-500, 0, 0), ProjectileType::BULLET, damage, false);
				}
			}

			if (attackVerable || bulletCnt >= 1 && !isShootingForward)
			{
				isShootingUp = true;
				// 위로 사격
				if (isAttacking)
				{
					ProjectilePool::GetInstance()->SummonProjectile(GetTransform()->GetWorldPosition() + Vector3d(37, 130, 0)
						, Vector3d(0, 500, 0), ProjectileType::BULLET, damage, false);
				}
				else
				{
					ProjectilePool::GetInstance()->SummonProjectile(GetTransform()->GetWorldPosition() + Vector3d(-35, 130, 0)
						, Vector3d(0, 500, 0), ProjectileType::BULLET, damage, false);
				}
			}
			SoundSystem::PlaySoundfile("sounds/enemy/Enemy_tripleshoot.wav");
			//isAttacking = false;
			bulletCnt++;
			if (bulletCnt == 3)
			{
				attackDelay2.Start();
				anim->LoadAnimationFromFile(L"sprites/enemy/shooter1/idle");
			}
			attackDelay.Start();
		}


		break;
	case Shooter1State::DEATH:
		animTimer.Update();
		if (stateJustChanged)
		{
			speed.x = 0;
			speed.y = 0;
			anim->LoadAnimationFromFile(L"sprites/enemy/shooter1/death");
			SoundSystem::PlaySoundfile("sounds/enemy/Enemy_dyingexplode.wav");
			animTimer.Start();
		}
		break;
	default:
		break;
	}

	stateJustChanged = false;

}


void Shooter1::OnCollisionEnter2D(const Collision2D& collision)
{
	auto box = collision.m_OtherCollider->GetGameObject()->GetComponent<Ground>();
	if (box != nullptr)
	{
		justLanded = true;
	}
}

void Shooter1::OnCollisionExit2D(const Collision2D& collision)
{
	auto box = collision.m_OtherCollider->GetGameObject()->GetComponent<Ground>();
	if (box != nullptr)
	{
		justFallen = true;
	}
}

void Shooter1::Damage(double damage)
{
	damageEffect.Start();
	Threat::Damage(damage);
	effect->GetGameObject()->SetSelfActive(true);
}

void Shooter1::OnDeath()
{
	changeState(Shooter1State::DEATH);
}
