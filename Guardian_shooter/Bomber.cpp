#include "Bomber.h"
#include "ProjectilePool.h"
#include "ThreatPool.h"
#include "Player.h"
#include "Ground.h"
#include "Timer.h"
#include "DebugObject.h"
#include "Guardian_shooter.h"
#include "DamageEffect.h"


Bomber::Bomber()
{
	idleTimer.duration = 1.0;
	idleTimer.onExpiration = [this]()
	{
		idleFinished = true;
	};
	attackCoolDownTimer.duration = 2.5;
	attackCoolDownTimer.onExpiration = [this]()
	{
		attackFinished = true;
		isBombShooted = false;
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

GameObject* Bomber::CreateBomber(Vector3d pos)
{
	// ThreatPool 없으면 생성.
	if (ThreatPool::GetInstance() == nullptr)
	{
		GameObject* threatPool = Scene::getCurrentScene()->AddGameObject();
		threatPool->AddComponent<ThreatPool>();
	}

	auto bomber = MapTool::GetInstance()->GetEnemyLayer()->AddGameObject();
	bomber->GetTransform()->SetWorldPosition(pos);
	auto shooterComponent = bomber->AddComponent<Bomber>();

	// hitbox
	bomber->AddComponent<BoxCollider2D>();
	bomber->GetComponent<BoxCollider2D>()->SetWidth(70);
	bomber->GetComponent<BoxCollider2D>()->SetHeight(170);
	DebugObject::CreateDebugRectImage(bomber, 70, 170, D2D1::ColorF::Red);
	DebugObject::CreateDebugText(bomber, [bomber]() {
		return to_wstring((int)bomber->GetComponent<Bomber>()->bomberState);
		}, Vector2d(0, 200), 20, D2D1::ColorF::Blue);

	//animation
	auto animObject = Scene::getCurrentScene()->AddGameObject(bomber);
	animObject->GetTransform()->position = Vector2d(0, 35);
	auto anim = animObject->AddComponent<D2DAnimatedSprite>();
	anim->SetWidth(256);
	anim->SetHeight(256);

	GameObject* effect = DamageEffect::CreateDamageEffect(Vector3d::zero);
	effect->SetParent(bomber);
	effect->SetSelfActive(false);
	shooterComponent->effect = effect->GetComponent<DamageEffect>();

	bomber->GetComponent<Bomber>()->Initialize(anim);

	return bomber;
}

void Bomber::Initialize(D2DAnimatedSprite* anim)
{
	hp = 3;
	invincible = false;
	threatType = ThreatType::SHOOTER_CURVE;
	bomberAnimation = anim;
	bomberAnimation->SetIsRepeating(false);
	bomberAnimation->LoadAnimationFromFile(L"sprites/enemy/shooter_curve/idle");
}

void Bomber::Damage(double damage)
{
	if (bomberState == BomberState::DEATH)
		return;
	damageEffect.Start();
	Threat::Damage(damage);
	effect->GetGameObject()->SetSelfActive(true);
	// 피격 효과 출력
}

void Bomber::Reset()
{
	idleFinished = true;
	attackFinished = true;
	isAttackReady = false;
	isBombShooted = false;
	isGroundStepped = false;
	bomberState = BomberState::IDLE;
	hp = 3;
}

void Bomber::OnDeath()
{
	bomberState = BomberState::DEATH;
	// 죽는 애니메이션 출력용 타이머 켜주기
	deathAnimTimer.Start();
	SoundSystem::PlaySoundfile("sounds/enemy/Enemy_dyingexplode.wav");
	// 애니메이션 불러오기
	bomberAnimation->SetIsRepeating(false);
	bomberAnimation->LoadAnimationFromFile(L"sprites/enemy/shooter_curve/death");
}

void Bomber::Update()
{
	bool stateJustChanged = false;

	idleTimer.Update();
	attackCoolDownTimer.Update();
	damageEffect.Update();

	if ((attackCoolDownTimer.elapsed >= 0.25 && attackCoolDownTimer.elapsed <= 0.3) && isBombShooted == false)
	{
		isAttackReady = true;
	}

	// state change
	switch (bomberState)
	{
		case BomberState::IDLE:
		{
			double distance = GetTransform()->GetWorldPosition().x - Player::GetPlayerPos().x;
			if (distance < 0)
			{
				horizontalView = 1;
			}
			else
			{
				horizontalView = -1;
			}
			if (idleFinished)
			{
				if ((abs(distance) <= attackRange) && (abs(distance) >= 90))
				{
					if (attackFinished)
					{
						if (isGroundStepped)
						{
							bomberState = BomberState::ATTACK;
							stateJustChanged = true;
						}
					}
				}
			}
		}
		break;
		case BomberState::ATTACK:
		{
			// 타이머 다 되면 IDLE로 변경
			if (attackFinished)
			{
				bomberState = BomberState::IDLE;
				stateJustChanged = true;
			}
		}
		break;
		case BomberState::DEATH:
			break;
		default:
			break;
	}

	GetTransform()->scale.x = horizontalView;

	switch (bomberState)
	{
		case BomberState::IDLE:
		{
			if (stateJustChanged)
			{
				idleTimer.Start();
				idleFinished = false;
				bomberAnimation->SetIsRepeating(true);
				bomberAnimation->LoadAnimationFromFile(L"sprites/enemy/shooter_curve/idle");
			}
			speed.x = 0;
			if (!isGroundStepped)
			{
				speed.y += -gravity * 10 * Time::GetDeltaTime();
			}
		}
		break;
		case BomberState::ATTACK:
		{
			if (stateJustChanged)
			{
				attackCoolDownTimer.Start();
				attackFinished = false;
				speed.x = 0;
				SoundSystem::PlaySoundfile("sounds/enemy/Enemy_grenadeshoot.mp3");
				bomberAnimation->SetIsRepeating(false);
				bomberAnimation->LoadAnimationFromFile(L"sprites/enemy/shooter_curve/attack");
			}
			if (isAttackReady)
			{
				isAttackReady = false;
				isBombShooted = true;
				// 풀에서 폭탄 소환
				Vector3d pos = GetTransform()->GetWorldPosition();
				Vector3d shootPos = Vector3d(pos.x + horizontalView * 45, pos.y + 165, 0);
				// 몬스터와 플레이어 사이의 거리
				double width = Player::GetPlayerPos().x - GetTransform()->GetWorldPosition().x;
				double height = Player::GetPlayerPos().y - GetTransform()->GetWorldPosition().y;				
				// 폭탄 던지는 각도 45도로 고정
				// 폭탄 쏠 때의 힘의 제곱은 distance * gravity / sin(2*theta)
				double force;
				if (width < 0)
				{
					if (height < 0)
						force = -sqrt(abs(width - height / 3) * gravity);
					else
						force = -sqrt(abs(width + height) * gravity);
				}
				else
				{
					if (height < 0)
						force = sqrt(abs(width + height / 3) * gravity);
					else
						force = sqrt(abs(width - height) * gravity);
				}
				ProjectilePool::GetInstance()->SummonProjectile(shootPos, Vector3d(force * cos(45.0) * 2.85, abs(force * sin(45.0) * 2.85), 0), ProjectileType::BOMB, damage, false);
			}
		}
		break;
		case BomberState::DEATH:
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

void Bomber::OnCollisionEnter2D(const Collision2D& collision)
{
	auto ground = collision.m_OtherCollider->GetGameObject()->GetComponent<Ground>();
	if (ground != nullptr)
	{
		speed.y = 0;
		isGroundStepped = true;
		steppedGrounds.insert(ground);
	}
}

void Bomber::OnCollisionExit2D(const Collision2D& collision)
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

