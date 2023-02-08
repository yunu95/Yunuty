#include "Player.h"
#include "Enemy.h"
#include "Ground.h"
#include "BulletPool.h"

Player* Player::instance = nullptr;

Player::Player()
{
	instance = this;
}

Vector3d Player::GetPlayerPos()
{
	return instance->GetTransform()->GetWorldPosition();
}

void Player::SetPlayerPos(Vector3d pos)
{
	instance->GetTransform()->SetWorldPosition(pos);
}

void Player::Initialize(D2DAnimatedSprite* upperAnimatedSprite, D2DAnimatedSprite* lowerAnimatedSprite)
{
	this->upperAnimatedSprite = upperAnimatedSprite;
	this->lowerAnimatedSprite = lowerAnimatedSprite;
}

void Player::Damage(double damage)
{
	hp -= damage;
	// 사망
	if (hp <= 0)
		GetGameObject()->SetSelfActive(false);
}

void Player::SetCombo(size_t combo)
{
	combo += 1;
	//타이머 초기화
}

void Player::Update()
{
	elapsed += Time::GetDeltaTime();

	// conditional
	switch (state)
	{
		case PlayerMovementState::IDLE:
			lowerAnimatedSprite->LoadAnimationFromFile(L"sprites/idle");
			speed.x = 0;
			break;
		case PlayerMovementState::WALK:
			lowerAnimatedSprite->LoadAnimationFromFile(L"sprites/move");
			if (speed.y > 0)
			{
				speed += Vector3d(0, -gravity, 0) * 10 * Time::GetDeltaTime();
			}
			break;
		case PlayerMovementState::JUMP:
			speed += Vector3d(0, -gravity, 0) * 10 * Time::GetDeltaTime();
			break;
		case PlayerMovementState::DASH:
			break;
		default:
			break;
	}
	switch (attackState)
	{
		case PlayerAttackState::NONE:
			break;
		case PlayerAttackState::SHOOT:
			upperAnimatedSprite->LoadAnimationFromFile(L"sprites/attack");
			break;
		case PlayerAttackState::MELEE:
			upperAnimatedSprite->LoadAnimationFromFile(L"sprites/melee");
			break;
		default:
			break;
	}
	GetTransform()->SetWorldPosition(GetTransform()->GetWorldPosition() + speed * Time::GetDeltaTime());

	const double horizontalAccel = 1000;

	if (D2DInput::isKeyDown(KeyCode::LeftArrow))
	{
		if (state == PlayerMovementState::IDLE)
			state = PlayerMovementState::WALK;
		speed.x = -horizontalAccel * 10 * Time::GetDeltaTime();
	}
	if (D2DInput::isKeyDown(KeyCode::RightArrow))
	{
		if (state == PlayerMovementState::IDLE)
			state = PlayerMovementState::WALK;
		speed.x = horizontalAccel * 10 * Time::GetDeltaTime();
	}
	if (D2DInput::isKeyLifted(KeyCode::LeftArrow))
	{
		if (state == PlayerMovementState::WALK)
			state = PlayerMovementState::IDLE;
		speed.x = 0;
	}
	if (D2DInput::isKeyLifted(KeyCode::RightArrow))
	{
		if (state == PlayerMovementState::WALK)
			state = PlayerMovementState::IDLE;
		speed.x = 0;
	}

	// jump
	if (D2DInput::isKeyDown(KeyCode::Space))
	{
		if (state != PlayerMovementState::JUMP)
		{
			state = PlayerMovementState::JUMP;
			speed.y = 500;
		}
	} 

	// shoot
	if (D2DInput::isKeyDown(KeyCode::Z))
	{
		if (elapsed >= 0.5f)
		{
			if (isEnemyClosed)
			{
				attackState = PlayerAttackState::MELEE;
				elapsed = 0.0f;
			}
			else
			{
				attackState = PlayerAttackState::SHOOT;
				BulletPool::instance->SummonBullet(GetTransform()->GetWorldPosition(), Vector3d(500.f, 0, 0), damage, true);
				elapsed = 0.0f;

			}
		}
	}
	if (D2DInput::isKeyLifted(KeyCode::Z))
	{
		attackState = PlayerAttackState::NONE;
	}

}

void Player::OnCollisionEnter2D(const Collision2D& collision)
{
	if (collision.m_Collider == playerMeleeRange)
	{
		auto closedEnemy = collision.m_OtherCollider->GetGameObject()->GetComponent<Enemy>();
		if (closedEnemy != nullptr)
		{
			isEnemyClosed = true;
		}
	}
	else
	{
		auto box = collision.m_OtherCollider->GetGameObject()->GetComponent<Ground>();
		if (box != nullptr)
		{
			speed.y = 0;
			state = PlayerMovementState::IDLE;
		}

	}

}

void Player::OnCollisionExit2D(const Collision2D& collision)
{
	if (collision.m_Collider == playerMeleeRange)
	{
		auto closedEnemy = collision.m_OtherCollider->GetGameObject()->GetComponent<Enemy>();
		if (closedEnemy != nullptr)
		{
			isEnemyClosed = false;
		}
	}

	else
	{
		auto box = collision.m_OtherCollider->GetGameObject()->GetComponent<Ground>();
		if (box != nullptr)
		{
			state = PlayerMovementState::JUMP;
		}
	}
}


void CreatePlayer(Scene* scene, Vector3d pos)
{
	auto player = scene->AddGameObject();
	//player->SetCollider(100, 50)
	player->AddComponent<Player>()->playerHitbox = player->AddComponent<BoxCollider2D>();
	player->GetComponent<BoxCollider2D>()->SetHeight(100);
	player->GetComponent<BoxCollider2D>()->SetWidth(50);
	// melee collider
	auto melee = player->AddComponent<BoxCollider2D>();
	melee->SetHeight(50);
	melee->SetWidth(100);
	player->GetComponent<Player>()->playerMeleeRange = melee;
	//player->AddComponent<Player>();
	player->GetTransform()->SetWorldPosition(pos);
	auto anim2 = player->AddComponent<D2DAnimatedSprite>();
	auto anim = player->AddComponent<D2DAnimatedSprite>();
	player->GetComponent<Player>()->Initialize(anim, anim2);
}

