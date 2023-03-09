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
	// ThreatPool ������ ����.
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
	// �ʱ�ȭ ����
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

	traceTimer.duration = 1.0;		// �̵����� ��ȯ�Ǵ� �ð�
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
	// ������Ʈ ����
	traceTimer.Update();
	attackDelay.Update();
	attackDelay2.Update();
	interrupted.Update();
	attackStop.Update();
	damageEffect.Update();


	// ���� ��ġ
	GetTransform()->SetWorldPosition(
		GetTransform()->GetWorldPosition()
		+ speed * Time::GetDeltaTime());

	// �����ϰ��� �ϴ� �÷��̾��� ��ġ
	Vector3d playerPos = Player::GetPlayerPos();

	// ���� ���� ���� ����
	if (abs(playerPos.x - GetTransform()->GetWorldPosition().x) < randHorRange &&
		abs(playerPos.y - GetTransform()->GetWorldPosition().y) < 400
		&& bulletCnt == 0)
	{
		attackHorable = true;
		//attackVerable = false;		// �ߺ� ��� ����
	}
	else
	{
		attackHorable = false;
	}

	// ���� ���� ���� ����
	if (abs(playerPos.x - GetTransform()->GetWorldPosition().x) < randVerRange &&
		playerPos.y - GetTransform()->GetWorldPosition().y > 400
		&& bulletCnt == 0)
	{
		attackVerable = true;
		//attackHorable = false;		// �ߺ� ��� ����
	}
	else
	{
		attackVerable = false;
	}

	if (attackHorable || attackVerable)
		canAttack = true;
	else
		canAttack = false;

	if (state != Shooter1State::ATTACK)			// ���� ���� ���� �ٲ� �� ī��Ʈ �ʱ�ȭ
		bulletCnt = 0;

	/// <summary>
/// ��ü�� ���°� �ٸ� ���·� �ٲ� �� �ִ����� ���� ������.
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
	/// �� ���¿� ���� �ൿ�� ���⼭ �����մϴ�.
	/// </summary>
	switch (state)
	{
	case Shooter1State::IDLE:
		/// <summary>
		/// ���°� �� IDLE ���·� �ٲ� ��쿡��, stateJustChanged�� ���� ���·� �� ���ǹ��� ����ϰ� �ȴ�.
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
		/// ���°� IDLE ������ �� �� �����Ӹ��� ȣ��Ǿ�� �ϴ� ������ ��� ���� �Ʒ��� �ۼ��ȴ�.
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
					// ������ ���
					ProjectilePool::GetInstance()->SummonProjectile(GetTransform()->GetWorldPosition() + Vector3d(100, 7, 0)
						, Vector3d(500, 0, 0), ProjectileType::BULLET, damage, false);
				}
				else
				{
					// ���� ���
					ProjectilePool::GetInstance()->SummonProjectile(GetTransform()->GetWorldPosition() + Vector3d(-100, 7, 0)
						, Vector3d(-500, 0, 0), ProjectileType::BULLET, damage, false);
				}
			}

			if (attackVerable || bulletCnt >= 1 && !isShootingForward)
			{
				isShootingUp = true;
				// ���� ���
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
