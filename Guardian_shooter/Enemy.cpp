#include "Enemy.h"
#include "EnemyPool.h"
#include "Ground.h"
#include "Player.h"
#include "ProjectilePool.h"
#include "Projectile.h"
#include "Timer.h"
#include "DebugObject.h"


Enemy::Enemy()
{
	damage = 10.0;
	hp = 3.0;
	attackTimer.duration = 1.5;		// �������� ��ȯ�Ǵ� �ð�
	traceTimer.duration = 1.2;		// �̵����� ��ȯ�Ǵ� �ð�
	attackDelay.duration = 1.0;
}

void Enemy::Initialize(D2DAnimatedSprite* animatedSprite)
{
	this->animatedSprite = animatedSprite;
}

GameObject* Enemy::CreateEnemy(Vector3d pos)
{
	auto enemy = Scene::getCurrentScene()->AddGameObject();
	enemy->AddComponent<BoxCollider2D>();
	enemy->GetComponent<BoxCollider2D>()->SetWidth(200);
	enemy->GetComponent<BoxCollider2D>()->SetHeight(100);

	enemy->AddComponent<Enemy>();
	enemy->AddComponent<EnemyPool>();

	enemy->GetTransform()->SetWorldPosition(pos);
	auto anim1 = enemy->AddComponent<D2DAnimatedSprite>();
	anim1->SetHeight(256);
	anim1->SetWidth(256);
	
	enemy->GetComponent<Enemy>()->Initialize(anim1);
	DebugObject::CreateDebugRectImage(enemy, 200, 100);
	return enemy;
}
void Enemy::changeState(EnemyState changedState)
{
	state = changedState;
	stateJustChanged = true;
}

void Enemy::Update()
{
	/// <summary>
	/// Ÿ�̸��� ������Ʈ �Լ��� ȣ���ϴ� �Ͱ�, ��ü�� �ӵ��� ��ü�� ��ġ�� ���ϴ� ���� �⺻ �������� �д�.
	/// </summary>
	attackTimer.Update();
	traceTimer.Update();
	attackDelay.Update();
	GetTransform()->SetWorldPosition(
		GetTransform()->GetWorldPosition() +
		speed * Time::GetDeltaTime());
	Vector3d playerVec = Player::GetPlayerPos();


	// ���� ���� ���� ����
	if (abs(playerVec.x - GetTransform()->GetWorldPosition().x) < 500 &&
		abs(playerVec.y - GetTransform()->GetWorldPosition().y) < 300)
	{
		attack_HorAble = true;
		attack_VerAble = false;		// �ߺ� ��� ����
	}
	else
	{
		attack_HorAble = false;
	}

	// ���� ���� ���� ����
	if (abs(playerVec.x - GetTransform()->GetWorldPosition().x) < 40 &&
		abs(playerVec.y - GetTransform()->GetWorldPosition().y) > 100)
	{
		attack_VerAble = true;
		attack_HorAble = false;		// �ߺ� ��� ����
	}
	else
	{
		attack_VerAble = false;
	}

	if (attack_HorAble || attack_VerAble)
		canAttack = true;
	else
		canAttack = false;

	/// <summary>
	/// ��ü�� ���°� �ٸ� ���·� �ٲ� �� �ִ����� ���� ������.
	/// </summary>\

	switch (state)
	{
	case EnemyState::IDLE:
		if (canAttack == false && traceTimer.isActive == false)
		{
			changeState(EnemyState::WALKING);
		}
		if (canAttack == true && traceTimer.isActive == false)
		{
			changeState(EnemyState::ATTACK);
		}
		if (justDamaged == true)
		{
			changeState(EnemyState::DEATH);
			justDamaged = false;
		}
		break;

	case EnemyState::FALLING:
		if (justLanded == true)
		{
			changeState(EnemyState::IDLE);
			justLanded = false;
		}
		if (justDamaged == true)
		{
			changeState(EnemyState::DEATH);
			justDamaged = false;
		}
		break;
	case EnemyState::WALKING:
		if (canAttack == true)
		{
			changeState(EnemyState::IDLE);
		}
		if (justFallen == true)
		{
			changeState(EnemyState::FALLING);
			justFallen = false;
		}
		if (justDamaged == true)
		{
			changeState(EnemyState::DEATH);
			justDamaged = false;
		}
		break;
	case EnemyState::ATTACK:
		if (canAttack == false)
		{
			changeState(EnemyState::WALKING);
		}
		if (justDamaged == true)
		{
			changeState(EnemyState::DEATH);
			justDamaged = false;
		}
		break;
	case EnemyState::DEATH:
		break;
	default:
		break;
	}
	/// <summary>
	/// �� ���¿� ���� �ൿ�� ���⼭ �����մϴ�.
	/// </summary>
	switch (state)
	{
	case EnemyState::IDLE:
		/// <summary>
		/// ���°� �� IDLE ���·� �ٲ� ��쿡��, stateJustChanged�� ���� ���·� �� ���ǹ��� ����ϰ� �ȴ�.
		/// </summary>
		if (stateJustChanged)
		{
			animatedSprite->LoadAnimationFromFile(L"sprites/enemy/shooter/idle");
			speed.x = 0;
			speed.y = 0;
			traceTimer.Start();
			//gravity = 0;
		}
		/// <summary>
		/// ���°� IDLE ������ �� �� �����Ӹ��� ȣ��Ǿ�� �ϴ� ������ ��� ���� �Ʒ��� �ۼ��ȴ�.
		/// </summary>


		break;
	case EnemyState::FALLING:
		speed += Vector3d(0, -gravity, 0) * 10 * Time::GetDeltaTime();

		if (stateJustChanged)
		{
			animatedSprite->LoadAnimationFromFile(L"sprites/enemy/shooter/fall");
			speed.x = 0;
		}
		break;
	case EnemyState::WALKING:
		if (stateJustChanged)
		{
			if (playerVec.x > this->GetTransform()->GetWorldPosition().x)
				speed.x = 300;
			if (playerVec.x < this->GetTransform()->GetWorldPosition().x)
				speed.x = -300;
			animatedSprite->LoadAnimationFromFile(L"sprites/enemy/shooter/move");
			speed.y = 0;
			traceTimer.Start();
		}

		break;
	case EnemyState::ATTACK:
		if (stateJustChanged)
		{
			animatedSprite->LoadAnimationFromFile(L"sprites/enemy/shooter/attack/forward");
			speed.x = 0;
			speed.y = 0;
		}
		if (!attackDelay.isActive)
		{
			if (attack_HorAble)
			{
				if (playerVec.x > GetTransform()->GetWorldPosition().x)
				{
					// ������ ���
					ProjectilePool::GetInstance()->SummonProjectile(GetTransform()->GetWorldPosition() - Vector3d(-200, 0, 0), Vector3d(500, 0, 0), ProjectileType::BULLET, 10, false);
	
				}
				else
				{
					// ���� ���
					ProjectilePool::GetInstance()->SummonProjectile(GetTransform()->GetWorldPosition() - Vector3d(200, 0, 0), Vector3d(-500, 0, 0), ProjectileType::BULLET, 10, false);
				}
			}

			if (attack_VerAble)
			{
				if (playerVec.y > GetTransform()->GetWorldPosition().y)
				{
					// ���� ���
					ProjectilePool::GetInstance()->SummonProjectile(GetTransform()->GetWorldPosition() - Vector3d(0, -50, 0), Vector3d(0, 500, 0), ProjectileType::BULLET, 10, false);
				}
				else
				{
					// �Ʒ��� ���
					ProjectilePool::GetInstance()->SummonProjectile(GetTransform()->GetWorldPosition() - Vector3d(0, 50, 0), Vector3d(0, -500, 0), ProjectileType::BULLET, 10, false);
				}
			}
			attackDelay.Start();
		}

		break;
	case EnemyState::DEATH:
		if (stateJustChanged)
		{
			speed.x = 0;
			speed.y = 0;

			hp -= damage;
			if (hp <= 0.0)
			{
				EnemyPool::pinstance->Retrieve(this);
				EnemyPool::pinstance->SummonEnemy(Vector3d(400, 400, 0));
				isSummoned = true;
			}

		}
		break;
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

//Vector3d Enemy::GetEnemyPos()
//{
//	return this->GetTransform()->GetWorldPosition();
//}

void Enemy::Damage(double damage)
{
	justDamaged = true;
	//hp -= damage;
	//if(hp <= 0)
		//GetGameObject()->SetSelfActive(false);

	Player::GetInstance()->AddCombo(1);
	Player::GetInstance()->SetGauge(1);
	//Player::instance->SetGauge(damage);
}

void Enemy::OnCollisionEnter2D(const Collision2D& collision)
{
	auto box = collision.m_OtherCollider->GetGameObject()->GetComponent<Ground>();
	if (box != nullptr)
	{
		justLanded = true;
	}
}

void Enemy::OnCollisionExit2D(const Collision2D& collision)
{
	auto box = collision.m_OtherCollider->GetGameObject()->GetComponent<Ground>();
	if (box != nullptr)
	{
		if (state == EnemyState::WALKING)
			justFallen = true;
	}
}

void Enemy::EnemyMove(Vector3d playerVec)
{


}

void Enemy::OnDeath()
{

}
