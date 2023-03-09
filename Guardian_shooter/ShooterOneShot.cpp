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
	// ThreatPool ������ ����.
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

	// �ǰ� ȿ�� ���

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
	// �״� �ִϸ��̼� ��¿� Ÿ�̸� ���ֱ�
	deathAnimTimer.Start();
	SoundSystem::PlaySoundfile("sounds/enemy/Enemy_dyingexplode.wav");
	// �ִϸ��̼� �ҷ�����
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
			// distance�� �����̸� ���Ͱ� �÷��̾� ���ʿ� �����Ƿ� ���ʹ� ���������� �������� �Ѵ�.
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
			// Ÿ�̸� �� �Ǹ� WALK ���·� ����
			// �Ǵ� �þ� �ȿ� �÷��̾ ������ WALK�� ���� - �÷��̾� �������� �ٰ�����
			if (idleFinished)
			{
				// ���� �þ� �ȿ� �÷��̾ ������
				if (abs(distance) <= sight)
				{
					isChasing = true;
					sight = 900.0;
					// �÷��̾ ���� ���� �ִٸ� �÷��̾� ��ġ�� ����
					if (Player::GetPlayerPos().y - GetTransform()->GetWorldPosition().y >= 85)
					{
						shooterState = ShooterOneShotState::CHASE;
						stateJustChanged = true;
						break;
					}
					// ���ݹ��� �ȿ� ������ ���ݸ��
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
				else // ���� �þ� �ȿ� �÷��̾ ������
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
			// ������ �������� FALL�� ����
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
			// Ÿ�̸� �� �Ǹ� IDLE�� ����
			if (walkFinished)
			{
				shooterState = ShooterOneShotState::IDLE;
				stateJustChanged = true;
			}
			// ������ �������� FALL�� ����
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
			// ���� ������ IDLE�� ����
			/// ���ο��� ��� FALL - IDLE ���� ������ �Ͼ
			/// �ȴ� �� FALL ���·� ���ͼ� ���� ��Ҵٸ�
			/// WALK ���·� �ٽ� �������ִ� ������ �߰�����.
			// �ȴ� �� ���������� WALK ���·� ����
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
			// Ÿ�̸� �� �Ǹ� IDLE�� ����
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
					// ���� ���
					bulletSpeed.y = horizontalView * bulletSpeed.x;
					bulletSpeed.x = 0;
					shooterAnimation->LoadAnimationFromFile(L"sprites/enemy/shooter/attack/upper");
					// Ǯ���� �Ѿ� ��ȯ
					Vector3d shootPos = Vector3d(pos.x + horizontalView * 37, pos.y + 165, 0);
					ProjectilePool::GetInstance()->SummonProjectile(shootPos, bulletSpeed, ProjectileType::BULLET, damage, false);
				}
				else
				{
					// �������� ���
					shooterAnimation->LoadAnimationFromFile(L"sprites/enemy/shooter/attack/forward");
					// Ǯ���� �Ѿ� ��ȯ
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
				/// Ǯ�� �ٽ� �־��ֱ�. ���� �ʱ�ȭ�� �ʿ�.
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

