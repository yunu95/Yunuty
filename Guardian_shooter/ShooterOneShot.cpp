#include "ShooterOneShot.h"
#include "ProjectilePool.h"
#include "ThreatPool.h"
#include "Player.h"
#include "Ground.h"
#include "DebugObject.h"
#include "Guardian_shooter.h"
#include "DamageEffect.h"

ShooterOneShot::ShooterOneShot()
{
	idleTimer.duration = 1.0;
	idleTimer.onExpiration = [this]()
	{
		idleFinished = true;
	};
	patrolTimer.duration = 1.2;
	patrolTimer.onExpiration = [this]()
	{
		walkFinished = true;
	};
	attackCoolDownTimer.duration = 2.0;
	attackCoolDownTimer.onExpiration = [this]()
	{
		attackFinished = true;
	};
	deathAnimTimer.duration = 0.38;
	deathAnimTimer.onExpiration = [this]()
	{
		isDying = true;
	};
	damageEffect.duration = 0.7;
	damageEffect.onExpiration = [this]()
	{
		effect->GetGameObject()->SetSelfActive(false);
	};
}

GameObject* ShooterOneShot::CreateShooterOneShot(Vector3d pos)
{
	// ThreatPool 없으면 생성.
	if (ThreatPool::GetInstance() == nullptr)
	{
		GameObject* threatPool = Scene::getCurrentScene()->AddGameObject();
		threatPool->AddComponent<ThreatPool>();
	}

	auto shooter = Scene::getCurrentScene()->AddGameObject();
	auto shooterComponent = shooter->AddComponent<ShooterOneShot>();

	shooter->GetTransform()->SetWorldPosition(pos);
	// hitbox
	shooter->AddComponent<BoxCollider2D>();
	shooter->GetComponent<BoxCollider2D>()->SetWidth(70);
	shooter->GetComponent<BoxCollider2D>()->SetHeight(170);
	DebugObject::CreateDebugRectImage(shooter, 70, 170, D2D1::ColorF::Red);
	DebugObject::CreateDebugText(shooter, [shooter]() {
		return to_wstring((int)shooter->GetComponent<ShooterOneShot>()->shooterState);
		}, Vector2d(0, 200), 20, D2D1::ColorF::Blue);

	//animation
	auto animObject = Scene::getCurrentScene()->AddGameObject(shooter);
	animObject->GetTransform()->position = Vector2d(0, 35);
	auto anim = animObject->AddComponent<D2DAnimatedSprite>();
	anim->SetWidth(256);
	anim->SetHeight(256);

	shooter->GetComponent<ShooterOneShot>()->Initialize(anim);

	GameObject* effect = DamageEffect::CreateDamageEffect(Vector3d::zero);
	effect->SetParent(shooter);
	effect->SetSelfActive(false);
	shooterComponent->effect = effect->GetComponent<DamageEffect>();

	return shooter;
}

void ShooterOneShot::Initialize(D2DAnimatedSprite* anim)
{
	hp = 2;
	invincible = false;
	threatType = ThreatType::SHOOTER_ONESHOT;
	shooterAnimation = anim;
	shooterAnimation->SetIsRepeating(false);
	shooterAnimation->LoadAnimationFromFile(L"sprites/enemy/shooter/fall");
}

void ShooterOneShot::Damage(double damage)
{
	if (shooterState == ShooterOneShotState::DEATH)
		return;
	damageEffect.Start();
	Threat::Damage(damage);
	effect->GetGameObject()->SetSelfActive(true);

	// 피격 효과 출력

}

void ShooterOneShot::Reset()
{
	idleFinished = true;
	walkFinished = true;
	attackFinished = true;
	isGroundStepped = false;
	isChasing = false;
	shooterState = ShooterOneShotState::FALL;
	hp = 2;
}

void ShooterOneShot::OnDeath()
{
	shooterState = ShooterOneShotState::DEATH;
	// 죽는 애니메이션 출력용 타이머 켜주기
	deathAnimTimer.Start();
	SoundSystem::PlaySoundfile("sounds/enemy/Enemy_dyingexplode.wav");
	// 애니메이션 불러오기
	shooterAnimation->SetIsRepeating(false);
	shooterAnimation->LoadAnimationFromFile(L"sprites/enemy/shooter/death");
}

void ShooterOneShot::Update()
{
	bool stateJustChanged = false;

	idleTimer.Update();
	patrolTimer.Update();
	attackCoolDownTimer.Update();
	damageEffect.Update();

	// state change
	switch (shooterState)
	{
		case ShooterOneShotState::IDLE:
		{
			double distance = GetTransform()->GetWorldPosition().x - Player::GetPlayerPos().x;
			// distance가 음수이면 몬스터가 플레이어 왼쪽에 있으므로 몬스터는 오른쪽으로 움직여야 한다.
			if (abs(distance) <= sight)
			{
				if (distance < 0)
				{
					if (horizontalView == -1)
					{
						horizontalView = 1;
						//idleTimer.duration -= 0.3;
					}
				}
				else
				{
					if (horizontalView == 1)
					{
						horizontalView = -1;
						//idleTimer.duration -= 0.3;
					}
				}
			}
			// 타이머 다 되면 WALK 상태로 변경
			// 또는 시야 안에 플레이어가 있으면 WALK로 변경 - 플레이어 뱡향으로 다가가기
			if (idleFinished)
			{
				// 적의 시야 안에 플레이어가 들어오면
				if (abs(distance) <= sight)
				{
					isChasing = true;
					sight = 900.0;
					// 플레이어가 몬스터 위에 있다면 플레이어 위치로 가기
					if (Player::GetPlayerPos().y - GetTransform()->GetWorldPosition().y >= 85)
					{
						shooterState = ShooterOneShotState::CHASE;
						stateJustChanged = true;
						break;
					}
					// 공격범위 안에 들어오면 공격모드
					if (abs(distance) <= attackRange)
					{
						if (attackFinished)
						{
							shooterState = ShooterOneShotState::ATTACK;
							stateJustChanged = true;
						}
					}
					else
					{
						if (walkFinished)
						{
							shooterState = ShooterOneShotState::PATROL;
							stateJustChanged = true;
						}
					}
				}
				else // 적의 시야 안에 플레이어가 없으면
				{
					isChasing = false;
					sight = 700.0;
					if (horizontalView == 1)
					{
						horizontalView = -1;
					}
					else
					{
						horizontalView = 1;
					}
					shooterState = ShooterOneShotState::PATROL;
					stateJustChanged = true;
				}
			}
			// 땅에서 떨어지면 FALL로 변경
			if (isGroundStepped == false)
			{
				shooterState = ShooterOneShotState::FALL;
				stateJustChanged = true;
				idleFinished = true;
			}
		}
		break;
		case ShooterOneShotState::PATROL:
		{
			// 타이머 다 되면 IDLE로 변경
			if (walkFinished)
			{
				shooterState = ShooterOneShotState::IDLE;
				stateJustChanged = true;
			}
			// 땅에서 떨어지면 FALL로 변경
			if (isGroundStepped == false)
			{
				shooterState = ShooterOneShotState::FALL;
				stateJustChanged = true;
			}
		}
		break;
		case ShooterOneShotState::CHASE:
		{
			double distance = GetTransform()->GetWorldPosition().x - Player::GetPlayerPos().x;
			double height = Player::GetPlayerPos().y - GetTransform()->GetWorldPosition().y;
			if (abs(distance) > sight)
			{
				shooterState = ShooterOneShotState::IDLE;
				stateJustChanged = true;
			}
			if ((abs(distance) <= 40) && (height >= 85))
			{
				if (attackFinished)
				{
					shooterState = ShooterOneShotState::ATTACK;
					stateJustChanged = true;
				}
			}
			if (height < 85)
			{
				if (attackFinished)
				{
					shooterState = ShooterOneShotState::IDLE;
					stateJustChanged = true;
				}
			}
		}
		break;
		case ShooterOneShotState::FALL:
		{
			// 땅에 닿으면 IDLE로 변경
			/// 경사로에서 계속 FALL - IDLE 상태 변경이 일어나
			/// 걷는 중 FALL 상태로 들어와서 땅을 밟았다면
			/// WALK 상태로 다시 변경해주는 로직을 추가했음.
			// 걷는 중 떨어졌으면 WALK 상태로 변경
			if (isGroundStepped)
			{
				if (walkFinished == false)
				{
					shooterState = ShooterOneShotState::PATROL;
					stateJustChanged = true;
				}
				else
				{
					shooterState = ShooterOneShotState::IDLE;
					stateJustChanged = true;
				}
			}
		}
		break;
		case ShooterOneShotState::ATTACK:
		{
			// 타이머 다 되면 IDLE로 변경
			if (attackFinished)
			{
				shooterState = ShooterOneShotState::IDLE;
				stateJustChanged = true;
			}
		}
		break;
		case ShooterOneShotState::DEATH:
			break;
		default:
			break;
	}

	GetTransform()->scale.x = horizontalView;

	switch (shooterState)
	{
		case ShooterOneShotState::IDLE:
		{
			if (stateJustChanged)
			{
				idleTimer.Start();
				idleFinished = false;
				shooterAnimation->SetIsRepeating(true);
				shooterAnimation->LoadAnimationFromFile(L"sprites/enemy/shooter/idle");
			}
			speed.x = 0;
			if (!isGroundStepped)
			{
				speed.y += -gravity * 10 * Time::GetDeltaTime();
			}
		}
		break;
		case ShooterOneShotState::PATROL:
		{
			if (stateJustChanged)
			{
				if (walkFinished)
					patrolTimer.Start();
				walkFinished = false;
				shooterAnimation->Play();
				shooterAnimation->SetIsRepeating(true);
				shooterAnimation->LoadAnimationFromFile(L"sprites/enemy/shooter/move");
			}
			speed.x = horizontalView * 500 * 10 * Time::GetDeltaTime();
			if (!isGroundStepped)
			{
				speed.y += -gravity * 10 * Time::GetDeltaTime();
			}
		}
		break;
		case ShooterOneShotState::CHASE:
			if (stateJustChanged)
			{
				shooterAnimation->Play();
				shooterAnimation->SetIsRepeating(true);
				shooterAnimation->LoadAnimationFromFile(L"sprites/enemy/shooter/move");
			}
			speed.x = horizontalView * 500 * 10 * Time::GetDeltaTime();
			if (!isGroundStepped)
			{
				speed.y += -gravity * 10 * Time::GetDeltaTime();
			}
			break;
		case ShooterOneShotState::FALL:
		{
			if (stateJustChanged)
			{
				shooterAnimation->SetIsRepeating(false);
				shooterAnimation->LoadAnimationFromFile(L"sprites/enemy/shooter/fall");
			}
			speed.y += -gravity * 10 * Time::GetDeltaTime();
		}
		break;
		case ShooterOneShotState::ATTACK:
		{
			if (stateJustChanged)
			{
				Vector3d bulletSpeed = Vector3d(400.f, 0, 0);
				bulletSpeed.x *= horizontalView;
				attackCoolDownTimer.Start();
				attackFinished = false;
				speed.x = 0;
				Vector3d pos = GetTransform()->GetWorldPosition();
				double distance = GetTransform()->GetWorldPosition().x - Player::GetPlayerPos().x;
				double height = Player::GetPlayerPos().y - GetTransform()->GetWorldPosition().y;
				SoundSystem::PlaySoundfile("sounds/enemy/Enemy_normalshoot.wav");
				shooterAnimation->SetIsRepeating(false);
				if ((abs(distance) <= 40) && height >= 75)
				{
					// 위로 쏘기
					bulletSpeed.y = horizontalView * bulletSpeed.x;
					bulletSpeed.x = 0;
					shooterAnimation->LoadAnimationFromFile(L"sprites/enemy/shooter/attack/upper");
					// 풀에서 총알 소환
					Vector3d shootPos = Vector3d(pos.x + horizontalView * 37, pos.y + 165, 0);
					ProjectilePool::GetInstance()->SummonProjectile(shootPos, bulletSpeed, ProjectileType::BULLET, damage, false);
				}
				else
				{
					// 정면으로 쏘기
					shooterAnimation->LoadAnimationFromFile(L"sprites/enemy/shooter/attack/forward");
					// 풀에서 총알 소환
					Vector3d shootPos = Vector3d(pos.x + horizontalView * 153, pos.y + 13, 0);
					ProjectilePool::GetInstance()->SummonProjectile(shootPos, bulletSpeed, ProjectileType::BULLET, damage, false);
				}
			}
			if (!isGroundStepped)
			{
				speed.y += -gravity * 10 * Time::GetDeltaTime();
			}
		}
		break;
		case ShooterOneShotState::DEATH:
			deathAnimTimer.Update();
			if (isDying)
			{
				isDying = false;
				/// 풀에 다시 넣어주기. 상태 초기화도 필요.
				ThreatPool::GetInstance()->Retrieve(this);
				Reset();
			}
			break;
		default:
			break;
	}
	GetTransform()->SetWorldPosition(GetTransform()->GetWorldPosition() + speed * Time::GetDeltaTime());
}

void ShooterOneShot::OnCollisionEnter2D(const Collision2D& collision)
{
	auto ground = collision.m_OtherCollider->GetGameObject()->GetComponent<Ground>();
	if (ground != nullptr)
	{
		speed.y = 0;
		isGroundStepped = true;
		steppedGrounds.insert(ground);
	}
}

void ShooterOneShot::OnCollisionExit2D(const Collision2D& collision)
{
	auto ground = collision.m_OtherCollider->GetGameObject()->GetComponent<Ground>();
	if (ground != nullptr)
	{
		steppedGrounds.erase(ground);
		if (steppedGrounds.empty())
		{
			isGroundStepped = false;
		}
	}
}

