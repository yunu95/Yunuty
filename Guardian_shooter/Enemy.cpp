#include "Enemy.h"
#include "EnemyPool.h"
#include "Ground.h"
#include "Player.h"
#include "BulletPool.h"
#include "Bullet.h"
#include "Timer.h"

Enemy* Enemy::instance = nullptr;

Enemy::Enemy()
{
	instance = this;

	attackTimer.duration = 3;
	attackTimer.isRepeat = false;
	attackTimer.onFinished = [this]() {state = EnemyState::ATTACK; };

	traceTimer.duration = 3;
	traceTimer.isRepeat = false;
	traceTimer.onFinished = [this]() { state = EnemyState::WALKING; };
}

void Enemy::Initialize(D2DAnimatedSprite* upperAnimatedSprite, D2DAnimatedSprite* lowerAnimatedSprite)
{
	this->upperAnimatedSprite = upperAnimatedSprite;
	this->lowerAnimatedSprite = lowerAnimatedSprite;
}

GameObject* CreateEnemy(Scene& scene, Vector3d pos)
{
	auto enemy = scene.AddGameObject();
	enemy->AddComponent<BoxCollider2D>();
	enemy->GetComponent<BoxCollider2D>()->SetWidth(100);
	enemy->GetComponent<BoxCollider2D>()->SetHeight(50);

	enemy->AddComponent<Enemy>();
	enemy->AddComponent<EnemyPool>();

	enemy->GetTransform()->SetWorldPosition(pos);
	auto anim1 = enemy->AddComponent<D2DAnimatedSprite>();
	auto anim2 = enemy->AddComponent<D2DAnimatedSprite>();
	enemy->GetComponent<Enemy>()->Initialize(anim1, anim2);
	return enemy;

}

void Enemy::Update()
{
	attackTimer.Update();
	traceTimer.Update();
	speed += Vector3d(0, -gravity, 0) * 10 * Time::GetDeltaTime();

	GetTransform()->SetWorldPosition(
		GetTransform()->GetWorldPosition() +
		speed * Time::GetDeltaTime());

	Vector3d playerVec = Player::GetPlayerPos();

	switch (state)
	{
	case EnemyState::IDLE:
		lowerAnimatedSprite->LoadAnimationFromFile(L"sprites/idle");
		break;

	case EnemyState::JUMPING:
		lowerAnimatedSprite->LoadAnimationFromFile(L"sprites/attack");
		speed.x = 0;
		break;

	case EnemyState::WALKING:
		if (playerVec.x > this->GetTransform()->GetWorldPosition().x)
			speed.x = 300;
		if (playerVec.x < this->GetTransform()->GetWorldPosition().x)
			speed.x = -300;
		break;

	case EnemyState::ATTACK:
		lowerAnimatedSprite->LoadAnimationFromFile(L"sprites/attack");
		BulletPool::instance->SummonBullet(GetTransform()->GetWorldPosition() - Vector3d(200, 0, 0), Vector3d(-500, 0, 0), 10, false);
		speed.x = 0;
		break;
	default:
		break;
	}

	
	if (abs(playerVec.x - GetTransform()->GetWorldPosition().x) > 500)	// 적의 x좌표가 플레이어의 500 밖에 있다
	{
		traceTimer.Start();
	}
	else		// 범위 안일 때
	{
		attackTimer.Start();
	}

}

Vector3d Enemy::GetEnemyPos()
{
	return instance->GetTransform()->GetWorldPosition();
}

void Enemy::Damage(double damage)
{
	hp -= damage;
	if (hp <= 0.0)
	{
		GetGameObject()->SetSelfActive(false);
	}
}

void Enemy::OnCollisionEnter2D(const Collision2D& collision)
{
	auto box = collision.m_OtherCollider->GetGameObject()->GetComponent<Ground>();
	if (box != nullptr)
	{
		speed.y = 0;
		gravity = 0;
		state = EnemyState::IDLE;
	}
}

void Enemy::OnCollisionExit2D(const Collision2D& collision)
{
	auto box = collision.m_OtherCollider->GetGameObject()->GetComponent<Ground>();
	if (box != nullptr)
	{
		gravity = 100;
		speed.y = -gravity;
		state = EnemyState::JUMPING;
	}
}

void Enemy::EnemyMove(Vector3d playerVec)
{
	

}
