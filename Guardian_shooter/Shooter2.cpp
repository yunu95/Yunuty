#include "Shooter2.h"
#include "DebugObject.h"
#include "Player.h"
#include "Ground.h"
#include "ProjectilePool.h"
#include "ThreatPool.h"
#include "DamageEffect.h"


void Shooter2::SetDirection()
{
	this->GetTransform()->scale.x *= -1;
}

void Shooter2::Buckshot(int bullets)
{
	//ProjectilePool::GetInstance()->SummonProjectile(GetTransform()->GetWorldPosition() - Vector3d(-100, 25, 0), Vector3d(200, 100, 0), ProjectileType::BULLET, damage, false);
}

double Shooter2::Radian(double degree)
{
	return degree / 57.3f;
}

void Shooter2::Initialize(D2DAnimatedSprite* animatedSprite)
{
	anim = animatedSprite;
}

GameObject* Shooter2::CreateShooter2(Vector3d pos)
{
	// ThreatPool ������ ����.
	if (ThreatPool::GetInstance() == nullptr)
	{
		GameObject* threatPool = Scene::getCurrentScene()->AddGameObject();
		threatPool->AddComponent<ThreatPool>();
	}

	auto shooter2 = Scene::getCurrentScene()->AddGameObject();

	shooter2->AddComponent<BoxCollider2D>();
	shooter2->GetComponent<BoxCollider2D>()->SetWidth(60);
	shooter2->GetComponent<BoxCollider2D>()->SetHeight(150);

	shooter2->GetTransform()->SetWorldPosition(pos);

	auto shooterComponent = shooter2->AddComponent<Shooter2>();

	GameObject* shooterImage = Scene::getCurrentScene()->AddGameObject(shooter2);
	auto anim1 = shooterImage->AddComponent<D2DAnimatedSprite>();
	shooterImage->GetTransform()->position = Vector3d(0, 40, 0);
	anim1->SetHeight(256);
	anim1->SetWidth(256);
	shooter2->GetComponent<Shooter2>()->Initialize(anim1);

	GameObject* effect = DamageEffect::CreateDamageEffect(Vector3d::zero);
	effect->SetParent(shooter2);
	effect->SetSelfActive(false);
	shooterComponent->effect = effect->GetComponent<DamageEffect>();

	DebugObject::CreateDebugRectImage(shooter2, 70, 170, D2D1::ColorF::Chocolate);

	return shooter2;
}

void Shooter2::Start()
{
	// �ʱ�ȭ ����
	hp = 2;
	damage = 1.f;
	gravity = 100;
	bulletCnt = 0;
	canAttack = false;
	attackFinished = false;
	justLanded = false;
	justDamaged = false;
	justFallen = false;
	stateJustChanged = true;
	isSummoned = false;

	state = Shooter2State::FALLING;

	traceTimer.duration = 2.7;		// �̵����� ��ȯ�Ǵ� �ð�
	attackDelay.duration = 1.1;
	attackDelay.onExpiration = [this]()
	{
		attackFinished = true;
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

void Shooter2::changeState(Shooter2State changedState)
{
	state = changedState;
	stateJustChanged = true;
}

void Shooter2::Update()
{
	// ������Ʈ ����
	traceTimer.Update();
	attackDelay.Update();
	damageEffect.Update();

	//interrupted.Update();

	// ���� ��ġ
	GetTransform()->SetWorldPosition(
		GetTransform()->GetWorldPosition()
		+ speed * Time::GetDeltaTime());

	// �����ϰ��� �ϴ� �÷��̾��� ��ġ
	Vector3d playerPos = Player::GetPlayerPos();

	// ���� ���� ���� ����
	if (abs(playerPos.x - GetTransform()->GetWorldPosition().x) < 800 && abs(playerPos.x - GetTransform()->GetWorldPosition().x) > 200)
	{
		canAttack = true;
	}

	else
		canAttack = false;

	if (state != Shooter2State::ATTACK)			// ���� ���� ���� �ٲ� �� ī��Ʈ �ʱ�ȭ
		bulletCnt = 0;

	/// <summary>
/// ��ü�� ���°� �ٸ� ���·� �ٲ� �� �ִ����� ���� ������.
/// </summary>\

	switch (state)
	{
	case Shooter2State::IDLE:
		//if (canAttack == false && traceTimer.isActive == false)
		//{
		//	changeState(Shooter2State::WALKING);
		//}
		if (canAttack == true && !traceTimer.isActive)
		{
			changeState(Shooter2State::ATTACK);
		}
		break;

	case Shooter2State::FALLING:
		if (justLanded == true)
		{
			changeState(Shooter2State::IDLE);
			justLanded = false;
		}
		break;
	case Shooter2State::WALKING:
		if (canAttack == true)
		{
			changeState(Shooter2State::IDLE);
		}
		if (justFallen == true)
		{
			changeState(Shooter2State::FALLING);
			justFallen = false;
		}
		break;
	case Shooter2State::ATTACK:
		if (attackFinished)
		{
			changeState(Shooter2State::IDLE);
		}
		break;
	case Shooter2State::DEATH:
		break;
	default:
		break;
	}
	/// <summary>
	/// �� ���¿� ���� �ൿ�� ���⼭ �����մϴ�.
	/// </summary>
	switch (state)
	{
	case Shooter2State::IDLE:
		/// <summary>
		/// ���°� �� IDLE ���·� �ٲ� ��쿡��, stateJustChanged�� ���� ���·� �� ���ǹ��� ����ϰ� �ȴ�.
		/// </summary>
		if (stateJustChanged)
		{
			anim->LoadAnimationFromFile(L"sprites/enemy/shooter2/idle");
			speed.x = 0;
			speed.y = 0;
			traceTimer.Start();
			//gravity = 0;
		}
		/// <summary>
		/// ���°� IDLE ������ �� �� �����Ӹ��� ȣ��Ǿ�� �ϴ� ������ ��� ���� �Ʒ��� �ۼ��ȴ�.
		/// </summary>


		break;
	case Shooter2State::FALLING:
		speed += Vector3d(0, -gravity, 0) * 10 * Time::GetDeltaTime();
		if (stateJustChanged)
		{
			anim->LoadAnimationFromFile(L"sprites/enemy/shooter2/fall");
			speed.x = 0;
		}
		break;
	case Shooter2State::WALKING:
		if (stateJustChanged)
		{
			if (playerPos.x > this->GetTransform()->GetWorldPosition().x)
			{
				speed.x = 300;
			}
			if (playerPos.x < this->GetTransform()->GetWorldPosition().x)
			{
				speed.x = -300;
			}
			anim->LoadAnimationFromFile(L"sprites/enemy/shooter2/move");
			speed.y = 0;
			traceTimer.Start();
		}

		if (playerPos.x > this->GetTransform()->GetWorldPosition().x)
			GetTransform()->scale.x = 1;

		if (playerPos.x < this->GetTransform()->GetWorldPosition().x)
			GetTransform()->scale.x = -1;


		if (edge)
		{
			speed.x *= -1;
			if (GetTransform()->scale.x == -1)
				GetTransform()->scale.x = 1;
			else
				GetTransform()->scale.x = -1;

			edge = false;
		}

		break;
	case Shooter2State::ATTACK:
		if (stateJustChanged)
		{
			anim->LoadAnimationFromFile(L"sprites/enemy/shooter2/attack");
			speed.x = 0;
			speed.y = 0;
			attackFinished = false;
			if (playerPos.x > GetTransform()->GetWorldPosition().x)
				GetTransform()->scale.x = 1;
			if (playerPos.x < GetTransform()->GetWorldPosition().x)
				GetTransform()->scale.x = -1;
			bulletCnt = 0;
		}
		if (bulletCnt < 1)
		{

			if (playerPos.x > GetTransform()->GetWorldPosition().x)
			{
				// ������ ���
				ProjectilePool::GetInstance()->SummonProjectile(GetTransform()->GetWorldPosition() - Vector3d(-100, -7, 0), Vector3d(200, 100, 0), ProjectileType::BULLET, damage, false);
				ProjectilePool::GetInstance()->SummonProjectile(GetTransform()->GetWorldPosition() - Vector3d(-100, -7, 0), Vector3d(200, -100, 0), ProjectileType::BULLET, damage, false);
				ProjectilePool::GetInstance()->SummonProjectile(GetTransform()->GetWorldPosition() - Vector3d(-100, -7, 0), Vector3d(200, 0, 0), ProjectileType::BULLET, damage, false);

			}
			else
			{
				// ���� ���
				ProjectilePool::GetInstance()->SummonProjectile(GetTransform()->GetWorldPosition() - Vector3d(100, -7, 0), Vector3d(-200, 100, 0), ProjectileType::BULLET, damage, false);
				ProjectilePool::GetInstance()->SummonProjectile(GetTransform()->GetWorldPosition() - Vector3d(100, -7, 0), Vector3d(-200, -100, 0), ProjectileType::BULLET, damage, false);
				ProjectilePool::GetInstance()->SummonProjectile(GetTransform()->GetWorldPosition() - Vector3d(100, -7, 0), Vector3d(-200, 0, 0), ProjectileType::BULLET, damage, false);
			}
			SoundSystem::PlaySoundfile("sounds/enemy/Enemy_tripleshoot.wav");
			bulletCnt++;
			if (bulletCnt == 1)
			{
				attackDelay.Start();
			}
		}

		break;
	case Shooter2State::DEATH:
		animTimer.Update();
		if (stateJustChanged)
		{
			speed.x = 0;
			speed.y = 0;
			anim->LoadAnimationFromFile(L"sprites/enemy/shooter2/death");
			SoundSystem::PlaySoundfile("sounds/enemy/Enemy_dyingexplode.wav");
			animTimer.Start();
		}
		break;

	case Shooter2State::PATROL:
		if (stateJustChanged)
		{

		}
	default:
		break;
	}

	if (isSummoned)
	{
		stateJustChanged = true;
		isSummoned = false;
	}
	else
	{
		stateJustChanged = false;
	}
	/*stateOnceChanged = true;*/
}


void Shooter2::OnCollisionEnter2D(const Collision2D& collision)
{
	auto box = collision.m_OtherCollider->GetGameObject()->GetComponent<Ground>();
	if (box != nullptr)
	{
		justLanded = true;
	}
}

void Shooter2::OnCollisionExit2D(const Collision2D& collision)
{ 
 	auto box = collision.m_OtherCollider->GetGameObject()->GetComponent<Ground>();
	if (box != nullptr)
	{
		justLanded = true;
	}
}

void Shooter2::Damage(double damage)
{
	damageEffect.Start();
	Threat::Damage(damage);
	effect->GetGameObject()->SetSelfActive(true);
}

void Shooter2::OnDeath()
{
	changeState(Shooter2State::DEATH);
}
