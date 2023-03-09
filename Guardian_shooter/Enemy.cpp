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
	attackTimer.duration = 1.5;		// 공격으로 전환되는 시간
	traceTimer.duration = 1.2;		// 이동으로 전환되는 시간
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
	/// 타이머의 업데이트 함수를 호출하는 것과, 객체의 속도를 객체의 위치에 더하는 것은 기본 동작으로 둔다.
	/// </summary>
	attackTimer.Update();
	traceTimer.Update();
	attackDelay.Update();
	GetTransform()->SetWorldPosition(
		GetTransform()->GetWorldPosition() +
		speed * Time::GetDeltaTime());
	Vector3d playerVec = Player::GetPlayerPos();


	// 수평 공격 가능 범위
	if (abs(playerVec.x - GetTransform()->GetWorldPosition().x) < 500 &&
		abs(playerVec.y - GetTransform()->GetWorldPosition().y) < 300)
	{
		attack_HorAble = true;
		attack_VerAble = false;		// 중복 사격 방지
	}
	else
	{
		attack_HorAble = false;
	}

	// 수직 공격 가능 범위
	if (abs(playerVec.x - GetTransform()->GetWorldPosition().x) < 40 &&
		abs(playerVec.y - GetTransform()->GetWorldPosition().y) > 100)
	{
		attack_VerAble = true;
		attack_HorAble = false;		// 중복 사격 방지
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
	/// 객체의 상태가 다른 상태로 바뀔 수 있는지를 먼저 따진다.
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
	/// 각 상태에 따른 행동은 여기서 진행합니다.
	/// </summary>
	switch (state)
	{
	case EnemyState::IDLE:
		/// <summary>
		/// 상태가 갓 IDLE 상태로 바뀐 경우에는, stateJustChanged가 참인 상태로 이 조건문을 통과하게 된다.
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
		/// 상태가 IDLE 상태일 때 매 프레임마다 호출되어야 하는 로직의 경우 여기 아래에 작성된다.
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
					// 오른쪽 사격
					ProjectilePool::GetInstance()->SummonProjectile(GetTransform()->GetWorldPosition() - Vector3d(-200, 0, 0), Vector3d(500, 0, 0), ProjectileType::BULLET, 10, false);
	
				}
				else
				{
					// 왼쪽 사격
					ProjectilePool::GetInstance()->SummonProjectile(GetTransform()->GetWorldPosition() - Vector3d(200, 0, 0), Vector3d(-500, 0, 0), ProjectileType::BULLET, 10, false);
				}
			}

			if (attack_VerAble)
			{
				if (playerVec.y > GetTransform()->GetWorldPosition().y)
				{
					// 위로 사격
					ProjectilePool::GetInstance()->SummonProjectile(GetTransform()->GetWorldPosition() - Vector3d(0, -50, 0), Vector3d(0, 500, 0), ProjectileType::BULLET, 10, false);
				}
				else
				{
					// 아래로 사격
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
