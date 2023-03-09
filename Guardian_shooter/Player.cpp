#include "Guardian_shooter.h"
#include "Player.h"
#include "ShooterOneShot.h"
#include "Drone.h"
#include "Projectile.h"
#include "Ground.h"
#include "ProjectilePool.h"
#include "DebugObject.h"
#include "PlayerGauge.h"
#include "CollisionEventDispatcher.h"

using namespace std;
using namespace YunutyEngine;
using namespace YunutyEngine::D2D;
bool Player::autoShot = true;


Player* Player::instance = nullptr;


Player::Player()
{
	instance = this;
	originCameraPos = GSCamera::GetInstance()->GetTransform()->GetWorldPosition();
	dashCoolDowmTimer.duration = 0.5;
	dashCoolDowmTimer.onUpdate = [this](double progress) {
		speed.x = horizontalView * (1 - progress) * dashSpeed;
	};
	dashCoolDowmTimer.onExpiration = [this]() {
		dashFinished = true;
		speed.x = 0;
	};
	attackCoolDownTimer.duration = 0.15;
	attackCoolDownTimer.onExpiration = [this]()
	{
		attackFinished = true;
	};
	meleeCoolDownTimer.duration = 0.5;
	meleeCoolDownTimer.onExpiration = [this]()
	{
		attackFinished = true;
		meleeFinished = true;
	};
	skillAfterImageTimer.duration = 0.5;
	skillAfterImageTimer.onUpdate = [this](double progress)
	{
		for (int i = 0; i < 5; i++)
		{
			auto progressOffset = i * 0.5 / 4.0;
			//auto alpha = (1 - ((progress - progressOffset)/0.5)*(progress - progressOffset)/0.5);
			auto alpha = (1 - ((progress)) * (progress));
			if (progressOffset > progress && !skillAfterImageAnimated[i])
			{
				skillRushTorsos[i]->GetGameObject()->SetSelfActive(true);
				skillRushTorsos[i]->Play();
				skillRushLegs[i]->Play();
				skillAfterImageAnimated[i] = true;
			}
			//skillRushTorsos[i]->GetTransform()->position = Vector2d(skillRushLength * (i + 1) / 5.0 * (1 - (1 - progress) * (1 - progress)), 45);
			skillRushTorsos[i]->GetTransform()->position = Vector2d(skillRushLength * (i + 1) / 5.0 * (1 - (1 - progress) * (1 - progress)), 45);
			skillRushTorsos[i]->GetTransform()->position.x *= skillRushDirection;
			skillRushTorsos[i]->color.a = alpha;
			skillRushLegs[i]->color.a = alpha;
		}
	};
	skillCoolDownTimer.onExpiration = [this]()
	{
		attackFinished = true;
		skillFinished = true;
	};
	skillCoolDownTimer.duration = 0.5;
	skillCoolDownTimer.onExpiration = [this]()
	{
		attackFinished = true;
		skillFinished = true;
	};
	comboTimer.duration = 5.0;
	comboTimer.onExpiration = [this]()
	{
		combo = 0;
	};
	invincibleTimerOnRevive.duration = 1.5;
	invincibleTimerOnRevive.onUpdate = [this](double progress)
	{
		if (upperAnimatedSprite->color.a <= 0.1)
		{
			upperAnimatedSprite->color.a += progress * 200;
			lowerAnimatedSprite->color.a += progress * 200;
		}
		else if (upperAnimatedSprite->color.a >= 0.7)
		{
			upperAnimatedSprite->color.a -= progress * 60;
			lowerAnimatedSprite->color.a -= progress * 60;
		}
	};
	invincibleTimerOnRevive.onExpiration = [this]()
	{
		isInvincible = false;
		upperAnimatedSprite->color.a = 1.0;
		lowerAnimatedSprite->color.a = 1.0;
	};
	invincibleTimer.duration = 0.35;
	invincibleTimer.onUpdate = [this](double progress)
	{
		if (upperAnimatedSprite->color.a <= 0.1)
		{
			upperAnimatedSprite->color.a += progress * 200;
			lowerAnimatedSprite->color.a += progress * 200;
		}
		else if (upperAnimatedSprite->color.a >= 0.7)
		{
			upperAnimatedSprite->color.a -= progress * 60;
			lowerAnimatedSprite->color.a -= progress * 60;
		}
	};
	invincibleTimer.onExpiration = [this]()
	{
		isInvincible = false;
		damagedAnimObj->SetSelfActive(false);
		upperAnimatedSprite->color.a = 1.0;
		lowerAnimatedSprite->color.a = 1.0;
	};
	deathAnimTimer.duration = 3.0;
	deathAnimTimer.onExpiration = [this]()
	{
		isDying = true;
	};
	skillCameraShakeTimer.duration = 0.2;
	skillCameraShakeTimer.onUpdate = [](double progress)
	{
		Vector3d cameraPos = GSCamera::GetInstance()->GetTransform()->GetWorldPosition();
		GSCamera::GetInstance()->GetTransform()->SetWorldPosition(Vector3d(cameraPos.x - 3, cameraPos.y + 3, 0));
	};
	skillCameraShakeTimer.onExpiration = [this]()
	{
		// 카메라 원래 위치로 되돌리기
		GSCamera::GetInstance()->GetTransform()->SetWorldPosition(originCameraPos);
	};
	parryCameraSlowTimer.duration = 0.2;
	parryCameraSlowTimer.onExpiration = [this]()
	{
		Time::SetTimeScale(1.0);
	};
	attackSpeedUpTimer.duration = 9.0;
	attackSpeedUpTimer.onExpiration = [this]()
	{
		attackSpeed = 1.0;
		isAttackSpeedUp = false;
	};
	droneLastTimer.duration = 6.5;
	droneLastTimer.onExpiration = [this]()
	{
		pdrone->GetGameObject()->SetSelfActive(false);
		isDroneSummoned = false;
	};

}

Vector3d Player::GetPlayerPos()
{
	return instance->GetTransform()->GetWorldPosition();
}

void Player::SetPlayerPos(Vector3d pos)
{
	instance->GetTransform()->SetWorldPosition(pos);
}

void Player::SetPlayerSpeed(Vector3d _speed)
{
	instance->speed += _speed;
}

char Player::GetPlayerDirection()
{
	return instance->horizontalView;
}

Vector3d Player::GetPlayerFootPos(Vector3d playerPos)
{
	return Vector3d(playerPos.x, playerPos.y - 75 * 0.9, 0);
}

void Player::Initialize(D2DAnimatedSprite* upperAnimatedSprite, D2DAnimatedSprite* lowerAnimatedSprite, D2DAnimatedSprite* damagedAnim)
{
	this->upperAnimatedSprite = upperAnimatedSprite;
	this->lowerAnimatedSprite = lowerAnimatedSprite;
	upperAnimatedSprite->LoadAnimationFromFile(L"sprites/player/torso/idle_forward");
	lowerAnimatedSprite->LoadAnimationFromFile(L"sprites/player/legs/jump");
	damagedAnimatedSprite = damagedAnim;
	damagedAnimatedSprite->LoadAnimationFromFile(L"sprites/player/effect");
	damagedAnimatedSprite->GetGameObject()->SetSelfActive(false);
}

void Player::Reset()
{
	state = PlayerMovementState::JUMP;
	speed = Vector3d(0, 0, 0);
	hp = maxHp;
	lifeCount = maxLifeCount;
	combo = 0;
	gaugeFilledSpeed = 1.0;
	horizontalView = 1;
	verticalView = 0;
	isInvincible = false;
	isMoving = false;
	dashFinished = true;
	isDashReady = true;
	groundStepped = false;
	attackFinished = true;
	attackJustStarted = false;
	meleeFinished = true;
	skillFinished = true;
	isDroneSummoned = false;
	attackSpeed = 1.0;
	upperAnimatedSprite->LoadAnimationFromFile(L"sprites/player/torso/idle_forward");
	lowerAnimatedSprite->LoadAnimationFromFile(L"sprites/player/legs/idle");
}

void Player::Damage()
{
	if (state == PlayerMovementState::DEATH)
		return;
	if (isInvincible)
		return;

	hp -= 1;
	combo = 0;

	if (hp <= 0)
	{
		--lifeCount;
		hp = maxHp;
	}

	// 사망
	if (lifeCount < 0)
	{
		Die();
		return;
	}

	// 피격 시 효과
	Hit();
	DebugObject::CreatePopUpCircle(GetTransform()->GetWorldPosition(), 25, 0.5, D2D1::ColorF::Blue);
}

void Player::Hit()
{
	// 빨갛게 깜빡이기

	// 0.2초 무적
	invincibleTimer.Start();
	isInvincible = true;
	damagedAnimObj->SetSelfActive(true);
	SoundSystem::PlaySoundfile("sounds/ui/electrical_short.wav");
}

void Player::Die()
{
	state = PlayerMovementState::DEATH;
	SoundSystem::PlaySoundfile("sounds/player/Player_death.wav");
	deathAnimTimer.Start();
	upperAnimatedSprite->LoadAnimationFromFile(L"sprites/player/torso/death");
	lowerAnimatedSprite->LoadAnimationFromFile(L"sprites/player/legs/death");
	upperAnimatedSprite->SetIsRepeating(false);
	lowerAnimatedSprite->SetIsRepeating(false);
}

void Player::Revive()
{
	Vector3d revivalPosition = GetRevivalPointPos();
	Revive(revivalPosition);
}

void Player::Revive(Vector3d pos)
{
	SetPlayerPos(pos);
	Reset();
	ResetPlayerStatus();
	GetGameObject()->SetSelfActive(true);
	isInvincible = true;
	invincibleTimerOnRevive.Start();
}

Vector3d Player::GetRevivalPointPos()
{
	auto revivalPos = steppedGroundAtLast->GetTransform()->GetWorldPosition();
	double factor = instance->GetPlayerPos().x < revivalPos.x ? -1 : 1;
	revivalPos += factor * (0.5 * steppedGroundAtLast->groundCollider->GetWidth() - 80) * steppedGroundAtLast->GetTransform()->GetWorldRotation().Right();
	revivalPos += Vector3d(0, 200, 0);
	return revivalPos;
}

void Player::AddCombo(size_t value)
{
	combo += value;
	if (combo >= 15)
		gaugeFilledSpeed = 1.5;
	else
		gaugeFilledSpeed = 1.0;

	//타이머 초기화
	comboTimer.Start();
}

void Player::SetGauge(size_t damage)
{
	if (attackState == PlayerAttackState::SKILL)
		return;
	// 게이지는 콤보 수와 데미지에 비례
	gauge += gaugeFilledSpeed * 10;
	if (gauge >= maxGauge)
	{
		if (skillCount < 3)
		{
			skillCount++;
			gauge = 0;
		}
		else
		{
			gauge = maxGauge - 1;
		}
	}
}

void Player::Heal(double value)
{
	hp += value;
	if (hp > maxHp)
		hp = maxHp;
}

void Player::AttackSpeedUp()
{
	isAttackSpeedUp = true;
	attackSpeed = 2.0;
	attackSpeedUpTimer.Start();
}

Vector3d Player::GetPlayerSpeed()
{
	return speed;
}

void Player::Update()
{
	if (skipFrame)
	{
		skipFrame = false;
		return;
	}
	bool stateJustChanged = false;
	invincibleTimerOnRevive.Update();

	comboTimer.Update();
	attackCoolDownTimer.Update();
	meleeCoolDownTimer.Update();
	skillCoolDownTimer.Update();
	skillAfterImageTimer.Update();
	dashCoolDowmTimer.Update();
	invincibleTimer.Update();
	droneLastTimer.Update();

	skillCameraShakeTimer.Update();
	parryCameraSlowTimer.Update();

	if (isAttackSpeedUp)
	{
		attackSpeedUpTimer.Update();
		if (attackSpeedUpTimer.isActive)
			attackCoolDownTimer.elapsed += Time::GetDeltaTime();
	}

	if (D2DInput::isKeyDown(KeyCode::UpArrow))
	{
		verticalView = 1;
	}
	else if (D2DInput::isKeyDown(KeyCode::DownArrow))
	{
		verticalView = -1;
	}
	else
	{
		verticalView = 0;
	}

	// state change
	switch (state)
	{
		case PlayerMovementState::IDLE:
			isMoving = false;
			isDashReady = true;
			if (D2DInput::isKeyDown(KeyCode::LeftArrow))
			{
				state = PlayerMovementState::WALK;
				stateJustChanged = true;
				isMoving = true;				
				horizontalView = -1;				
			}
			else if (D2DInput::isKeyDown(KeyCode::RightArrow))
			{
				state = PlayerMovementState::WALK;
				stateJustChanged = true;
				isMoving = true;				
				horizontalView = 1;				
			}
			if (D2DInput::isKeyDown(KeyCode::Space))
			{
				state = PlayerMovementState::JUMP;
				stateJustChanged = true;
			}
			else if (D2DInput::isKeyDown(KeyCode::LShift))
			{
				if (isDashReady)
				{
					state = PlayerMovementState::DASH;
					stateJustChanged = true;
				}
			}
			break;

		case PlayerMovementState::WALK:
			if (D2DInput::isKeyDown(KeyCode::LeftArrow))
			{				
				horizontalView = -1;				
				isMoving = true;
			}
			else if (D2DInput::isKeyDown(KeyCode::RightArrow))
			{				
				horizontalView = 1;
				isMoving = true;
			}
			else
			{
				state = PlayerMovementState::IDLE;
				stateJustChanged = true;
			}
			if (D2DInput::isKeyDown(KeyCode::Space))
			{
				state = PlayerMovementState::JUMP;
				stateJustChanged = true;
			}
			else if (D2DInput::isKeyDown(KeyCode::LShift))
			{
				if (isDashReady)
				{
					state = PlayerMovementState::DASH;
					stateJustChanged = true;
				}
			}
			break;

		case PlayerMovementState::JUMP:
			// 방향키 입력을 통한 바라보고 있는 방향부터 체크한다.
			// 제자리점프와 이동 시 점프를 구분하기 위해서이다. (애니메이션 출력이 달라짐.)
			if (D2DInput::isKeyDown(KeyCode::LeftArrow))
			{				
				horizontalView = -1;
				isMoving = true;
			}
			else if (D2DInput::isKeyDown(KeyCode::RightArrow))
			{				
				horizontalView = 1;				
				isMoving = true;
			}
			if (D2DInput::isKeyDown(KeyCode::LShift))
			{
				if (isDashReady)
				{
					state = PlayerMovementState::DASH;
					stateJustChanged = true;
				}
			}
			if (groundStepped)
			{
				SoundSystem::PlaySoundfile("sounds/player/Player_Landing.wav");
				state = PlayerMovementState::IDLE;
				stateJustChanged = true;
			}
			break;
		case PlayerMovementState::DASH:
			if (dashFinished)
			{
				if (steppedGrounds.empty())
				{
					state = PlayerMovementState::JUMP;
				}
				else
				{
					state = PlayerMovementState::IDLE;
				}
				stateJustChanged = true;
			}
			break;

		case PlayerMovementState::DEATH:
			break;

		default:
			break;
	}

	// 좌우반전
	playerChildObject->GetTransform()->scale.x = horizontalView;


	switch (attackState)
	{
		case PlayerAttackState::NONE:
			if (autoShot && D2DInput::isKeyDown(KeyCode::Z) ||
				!autoShot && D2DInput::isKeyPushed(KeyCode::Z))
			{
				if (!attackJustStarted && attackFinished)
				{
					attackState = PlayerAttackState::SHOOT;
					attackJustStarted = true;					
				}
			}
			else if (D2DInput::isKeyDown(KeyCode::X))
			{
				if (meleeFinished)
				{
					attackState = PlayerAttackState::MELEE;
					attackJustStarted = true;
				}
			}
			else if (D2DInput::isKeyDown(KeyCode::C))
			{
				if (skillCount >= 1)
				{
					attackState = PlayerAttackState::SKILL;
					attackJustStarted = true;
				}
			}
			else if (D2DInput::isKeyDown(KeyCode::V))
			{
				if (skillCount >= 2)
				{
					attackState = PlayerAttackState::SKILL2;
					attackJustStarted = true;
					isDroneSummoned = true;
				}
			}
			break;
		case PlayerAttackState::SHOOT:
			if (!attackJustStarted && attackFinished)
			{
				attackState = PlayerAttackState::NONE;
			}
			break;
		case PlayerAttackState::MELEE:
			if (meleeFinished)
			{
				attackState = PlayerAttackState::NONE;
			}
			break;
		case PlayerAttackState::SKILL:
			if (skillFinished)
				attackState = PlayerAttackState::NONE;
			break;
		case PlayerAttackState::SKILL2:
			if (skillFinished)
				attackState = PlayerAttackState::NONE;
			break;
		default:
			break;
	}

	// conditional
	switch (state)
	{
		case PlayerMovementState::IDLE:
			if (stateJustChanged || (meleeFinished && skillFinished))
			{
				lowerAnimatedSprite->SetIsRepeating(false);
				lowerAnimatedSprite->LoadAnimationFromFile(L"sprites/player/legs/idle");
			}
			speed.x = 0;
			speed.y = 0;
			break;
		case PlayerMovementState::WALK:
			if (stateJustChanged)
			{
				lowerAnimatedSprite->SetIsRepeating(true);
				lowerAnimatedSprite->Play();
				lowerAnimatedSprite->LoadAnimationFromFile(L"sprites/player/legs/move");
			}
			speed.y = 0;
			speed.x = horizontalView * horizontalWalkSpeed;
			break;
		case PlayerMovementState::JUMP:
		{
			if (isMoving)
				speed.x = horizontalView * horizontalWalkSpeed;
			else
				speed.x = 0;

			if (stateJustChanged)
			{
				if (groundStepped)
				{
					speed.y = jumpForce * 20;
					groundStepped = false;
				}
				SoundSystem::PlaySoundfile("sounds/player/Player_Jump_Up.wav");
				// animation
				lowerAnimatedSprite->SetIsRepeating(false);
				if (!isMoving)
					lowerAnimatedSprite->LoadAnimationFromFile(L"sprites/player/legs/jump");
				else
					lowerAnimatedSprite->LoadAnimationFromFile(L"sprites/player/legs/jump_forward");
			}
			speed.y += -gravity * 20 * Time::GetDeltaTime();
		}
		break;

		case PlayerMovementState::DASH:
			if (stateJustChanged)
			{
				if (isDashReady)
				{
					//timer on
					dashCoolDowmTimer.Start();
					speed.x = horizontalView * dashSpeed;
					SoundSystem::PlaySoundfile("sounds/player/Player_dash.wav");
					upperAnimatedSprite->SetIsRepeating(false);
					upperAnimatedSprite->LoadAnimationFromFile(L"sprites/player/torso/dash");
					lowerAnimatedSprite->LoadAnimationFromFile(L"sprites/player/legs/dash");
					dashFinished = false;
					isDashReady = false;
				}
			}
			speed.y = 0;
			break;
		case PlayerMovementState::DEATH:
			deathAnimTimer.Update();
			if (isDying)
			{
				isDying = false;
				GetGameObject()->SetSelfActive(false);
			}
			break;
		default:
			break;
	}

	// 죽은 상태라면 상체 애니메이션 갱신하지 않음.
	if (state == PlayerMovementState::DEATH)
		return;

	switch (attackState)
	{
		case PlayerAttackState::NONE:
			upperAnimatedSprite->SetIsRepeating(true);
			if (dashFinished)
			{
				if (verticalView == 1)
				{
					upperAnimatedSprite->LoadAnimationFromFile(L"sprites/player/torso/idle_up");
				}
				else if (verticalView == -1)
				{
					if (groundStepped)
					{
						upperAnimatedSprite->LoadAnimationFromFile(L"sprites/player/torso/idle_forward");
					}
				}
				else
				{
					upperAnimatedSprite->LoadAnimationFromFile(L"sprites/player/torso/idle_forward");
				}
			}
			break;
		case PlayerAttackState::SHOOT:
			if (attackJustStarted)
			{
				SoundSystem::PlaySoundfile("sounds/player/Player_shoot.wav");
				Vector3d bulletSpeed = Vector3d(1600.f, 0, 0);
				attackCoolDownTimer.Start();
				bulletSpeed.x *= horizontalView;
				upperAnimatedSprite->SetIsRepeating(false);
				if (verticalView == 1)
				{
					upperAnimatedSprite->LoadAnimationFromFile(L"sprites/player/torso/fire_up");
					bulletSpeed.y = horizontalView * bulletSpeed.x;
					bulletSpeed.x = 0;
				}
				else if (verticalView == -1)
				{
					if (!groundStepped)
					{
						upperAnimatedSprite->LoadAnimationFromFile(L"sprites/player/torso/fire_down");
						bulletSpeed.y = -horizontalView * bulletSpeed.x;
						bulletSpeed.x = 0;
					}
					else
					{
						upperAnimatedSprite->LoadAnimationFromFile(L"sprites/player/torso/fire_forward");
					}
				}
				else
				{
					upperAnimatedSprite->LoadAnimationFromFile(L"sprites/player/torso/fire_forward");
				}
				attackJustStarted = false;
				attackFinished = false;
				Vector3d pos = GetTransform()->GetWorldPosition();
				if (verticalView == 0)
				{
					if (horizontalView == 1)
					{
						ProjectilePool::GetInstance()->SummonProjectile(Vector3d(pos.x + 88, pos.y + 32, 0), bulletSpeed, ProjectileType::BULLET, shootDamage, true);
					}
					else
						ProjectilePool::GetInstance()->SummonProjectile(Vector3d(pos.x - 108, pos.y + 32, 0), bulletSpeed, ProjectileType::BULLET, shootDamage, true);
				}
				else if (verticalView == 1)
				{
					if (horizontalView == 1)
						ProjectilePool::GetInstance()->SummonProjectile(Vector3d(pos.x + 5, pos.y + 148, 0), bulletSpeed, ProjectileType::BULLET, shootDamage, true);
					else
						ProjectilePool::GetInstance()->SummonProjectile(Vector3d(pos.x - 5, pos.y + 148, 0), bulletSpeed, ProjectileType::BULLET, shootDamage, true);
				}
				else     //verticalView == -1
				{
					if (!groundStepped)
					{
						if (horizontalView == 1)
							ProjectilePool::GetInstance()->SummonProjectile(Vector3d(pos.x + 5, pos.y - 108, 0), bulletSpeed, ProjectileType::BULLET, shootDamage, true);
						else
							ProjectilePool::GetInstance()->SummonProjectile(Vector3d(pos.x + 5, pos.y - 108, 0), bulletSpeed, ProjectileType::BULLET, shootDamage, true);
					}
					else
					{
						if (horizontalView == 1)
							ProjectilePool::GetInstance()->SummonProjectile(Vector3d(pos.x + 88, pos.y + 32, 0), bulletSpeed, ProjectileType::BULLET, shootDamage, true);
						else
							ProjectilePool::GetInstance()->SummonProjectile(Vector3d(pos.x - 108, pos.y + 32, 0), bulletSpeed, ProjectileType::BULLET, shootDamage, true);
					}
				}
			}
			break;

		case PlayerAttackState::MELEE:
			if (attackJustStarted)
			{
				meleeCoolDownTimer.Start();
				SoundSystem::PlaySoundfile("sounds/player/Player_knife.wav");
				upperAnimatedSprite->SetIsRepeating(false);
				upperAnimatedSprite->LoadAnimationFromFile(L"sprites/player/torso/melee");
				if (!isMoving)
				{
					lowerAnimatedSprite->SetIsRepeating(false);
					lowerAnimatedSprite->LoadAnimationFromFile(L"sprites/player/legs/melee/stay");
				}
				else
				{
					if (!groundStepped)
						lowerAnimatedSprite->LoadAnimationFromFile(L"sprites/player/legs/melee/move");
				}
				attackJustStarted = false;
				attackFinished = false;
				meleeFinished = false;
				for (auto each : closeEnemies)
				{
					if (each->hp > 0)
						each->Damage(meleeDamage);
				}
			}
			break;

		case PlayerAttackState::SKILL:
			if (attackJustStarted)
			{
				skillCoolDownTimer.Start();
				//skillCameraShakeTimer.Start();
				skillAfterImageTimer.Start();
				if (projectilesToParry.empty())
					SoundSystem::PlaySoundfile("sounds/player/Player_gaugeatk.wav");
				else
				{
					for (auto each : projectilesToParry)
					{
						if (!each->IsTimerOn())
						{
							SoundSystem::PlaySoundfile("sounds/player/Player_gaugeatkp.wav");
							parryCameraSlowTimer.Start();
							Time::SetTimeScale(0.5);
							break;
						}
					}
				}
				upperAnimatedSprite->SetIsRepeating(false);
				upperAnimatedSprite->LoadAnimationFromFile(L"sprites/player/torso/skill");
				if (!isMoving)
				{
					lowerAnimatedSprite->SetIsRepeating(false);
					lowerAnimatedSprite->LoadAnimationFromFile(L"sprites/player/legs/melee/stay");
				}
				else
				{
					if (!groundStepped)
						lowerAnimatedSprite->LoadAnimationFromFile(L"sprites/player/legs/melee/move");
				}
				attackJustStarted = false;
				attackFinished = false;
				skillFinished = false;
				skillCount--;
				// damage
				for (auto each : closeEnemiesSetForSkill)
				{
					each->Damage(skillDamage);
				}
				// parry
				for (auto each : projectilesToParry)
				{
					if (each->IsTimerOn())
					{
						continue;
					}
					each->isPlayerProj = true;
					if (each->projType == ProjectileType::BOMB)
					{
						each->speed *= -1;
					}
					else if (each->projType == ProjectileType::BOSSBULLET)
					{
						each->speed = 2.7 * (Boss::GetBossPos() - GetPlayerPos());
					}
					else
					{
						each->speed *= -8;
					}
					each->damage = 7;
				}
				projectilesToParry.clear();
				skillRushDirection = horizontalView;
				for (int i = 0; i < 5; i++)
				{
					skillRushTorsos[i]->GetTransform()->scale.x = horizontalView;
					skillAfterImageAnimated[i] = false;
				}
			}
			break;
		case PlayerAttackState::SKILL2:
			if (attackJustStarted)
			{
				if (isDroneSummoned)
				{					
					SoundSystem::PlaySoundfile("sounds/player/Summon_drone.wav");
					pdrone->GetGameObject()->SetSelfActive(true);
					droneLastTimer.Start();					
				}
				attackJustStarted = false;
				attackFinished = true;
				skillFinished = true;
				skillCount -= 2;
			}
			break;
		default:
			break;
	}
	// 플랫폼과 맞닿아 있는 경우, 플레이어의 x축 속도는 플레이어의 각도에 따라 달라진다.
	if ((GetMovementState() == PlayerMovementState::WALK || GetMovementState() == PlayerMovementState::DASH) && !steppedGrounds.empty())
	{
		auto originalSpeed = speed;
		auto rotatedSpeed = (*steppedGrounds.begin())->GetTransform()->GetWorldRotation() * Vector3d(speed.x, 0, 0);
		speed.x = 0;
		speed += rotatedSpeed;
		GetTransform()->SetWorldPosition(GetTransform()->GetWorldPosition() + speed * Time::GetDeltaTime());
		speed = originalSpeed;
	}
	else
		GetTransform()->SetWorldPosition(GetTransform()->GetWorldPosition() + speed * Time::GetDeltaTime());


	// 플레이어가 카메라 벗어나면 벗어나기 전 위치로 되돌려준다.
	if (GSCamera::GetInstance() && steppedGroundAtLast && !GSCamera::GetInstance()->isRecovering())
	{
		if ((instance->GetPlayerPos().x - 30) <= (GSCamera::GetInstance()->GetTransform()->GetWorldPosition().x - 1920 / 2.0))
		{
			instance->SetPlayerPos(Vector3d(GSCamera::GetInstance()->GetTransform()->GetWorldPosition().x - 1920 / 2.0 + 30, instance->GetPlayerPos().y, 0));
		}
		else if ((instance->GetPlayerPos().x + 30) >= (GSCamera::GetInstance()->GetTransform()->GetWorldPosition().x + 1920 / 2.0))
		{
			instance->SetPlayerPos(Vector3d(GSCamera::GetInstance()->GetTransform()->GetWorldPosition().x + 1920 / 2.0 - 30, instance->GetPlayerPos().y, 0));
		}
		if ((instance->GetPlayerFootPos().y) <= (GSCamera::GetInstance()->GetTransform()->GetWorldPosition().y - 840))
		{
			Damage();
			auto revivalPosition = GetRevivalPointPos();
			if (lifeCount >= 0)
				SetPlayerPos(revivalPosition);
		}
	}
	lastFramePlayerPos = instance->GetPlayerPos();
}

void Player::OnCollisionEnter2D(const Collision2D& collision)
{
	if (collision.m_Collider == playerMeleeHitBox)
	{
		auto closeEnemy = collision.m_OtherCollider->GetGameObject()->GetComponent<Threat>();
		if (closeEnemy != nullptr)
		{
			closeEnemies.insert(closeEnemy);
		}
	}

	if (collision.m_Collider == playerSkillHitBox)
	{
		auto closeEnemy = collision.m_OtherCollider->GetGameObject()->GetComponent<Threat>();
		if (closeEnemy != nullptr)
		{
			closeEnemiesSetForSkill.insert(closeEnemy);
		}
		auto closeBullet = collision.m_OtherCollider->GetGameObject()->GetComponent<Projectile>();
		if (closeBullet != nullptr)
		{
			if (closeBullet->isPlayerProj == false)
				projectilesToParry.insert(closeBullet);
		}
	}

	if (collision.m_Collider == playerHitbox)
	{
		auto box = collision.m_OtherCollider->GetGameObject()->GetComponent<Ground>();
		if (box != nullptr && (
			(!box->isPenetrable && box->isSteppable()) ||
			(box->isPenetrable && box->isSteppable() && box->isUnderFoot(lastFramePlayerPos))
			)
			)
		{
			steppedGroundAtLast = box;
			speed.y = 0;
			groundStepped = true;
			steppedGrounds.insert(box);
			DebugObject::CreatePopUpCircle(box->GetTransform()->position);
		}
	}
}
void Player::OnCollisionStay2D(const Collision2D& collision)
{
	auto box = collision.m_OtherCollider->GetGameObject()->GetComponent<Ground>();
	// 플레이어가 플랫폼과 비비고 있을 때 떨어지는걸 방지하기 위한 코드입니다.
	if (steppedGrounds.find(box) != steppedGrounds.end() && GetMovementState() == PlayerMovementState::JUMP)
	{
		auto platformUp = box->GetTransform()->GetWorldRotation().Up();
		if (Vector3d::Dot(speed, platformUp) < 0 && (
			(!box->isPenetrable && box->isSteppable()) ||
			(box->isPenetrable && box->isSteppable() && box->isUnderFoot(GetPlayerFootPos())))
			)
		{
			speed.y = 0;
			groundStepped = true;
			steppedGrounds.insert(box);
		}
	}
}

void Player::OnCollisionExit2D(const Collision2D& collision)
{
	if (collision.m_Collider == playerMeleeHitBox)
	{
		auto closeEnemy = collision.m_OtherCollider->GetGameObject()->GetComponent<Threat>();
		if (closeEnemy != nullptr)
		{
			closeEnemies.erase(closeEnemy);
		}
	}

	if (collision.m_Collider == playerSkillHitBox)
	{
		auto closeEnemy = collision.m_OtherCollider->GetGameObject()->GetComponent<Threat>();
		if (closeEnemy != nullptr)
		{
			closeEnemiesSetForSkill.erase(closeEnemy);
		}
		auto closeBullet = collision.m_OtherCollider->GetGameObject()->GetComponent<Projectile>();
		if (closeBullet != nullptr)
		{
			projectilesToParry.erase(closeBullet);
		}
	}

	if (collision.m_Collider == playerHitbox)
	{
		auto box = collision.m_OtherCollider->GetGameObject()->GetComponent<Ground>();
		if (box != nullptr)
		{
			steppedGrounds.erase(box);
			if (steppedGrounds.empty())
			{
				state = PlayerMovementState::JUMP;
				groundStepped = false;
			}
		}
	}
}


GameObject* Player::CreatePlayer(Vector3d pos)
{
	if (instance)
	{
		instance->Revive(pos);
		instance->gauge = 0;
		instance->skillCount = 0;
		return instance->GetGameObject();
	}
	auto player = Scene::getCurrentScene()->AddGameObject(gs_map::MapTool::GetInstance()->GetPlayerLayer());
	auto playerComp = player->AddComponent<Player>();
	playerComp->playerHitbox = player->AddComponent<BoxCollider2D>();
	player->GetComponent<BoxCollider2D>()->SetHeight(150);
	player->GetComponent<BoxCollider2D>()->SetWidth(60);

	DebugObject::CreateDebugRectImage(player, 60, 150, D2D1::ColorF::Blue);

	playerComp->GetInstance()->playerChildObject = Scene::getCurrentScene()->AddGameObject(player);
	GameObject* playerObj = playerComp->GetInstance()->playerChildObject;

	//melee hit box
	auto meleeObject = Scene::getCurrentScene()->AddGameObject(playerObj);
	meleeObject->GetTransform()->position = Vector2d(65, 30);
	auto meleeDispatcher = meleeObject->AddComponent<BoxCollider2D>();
	meleeDispatcher->SetHeight(200);
	meleeDispatcher->SetWidth(150);
	meleeDispatcher->GetGameObject()->AddComponent<CollisionEventDispatcher>()->target = playerComp;

	playerComp->playerMeleeHitBox = meleeDispatcher;
	DebugObject::CreateColliderImage(playerComp->playerMeleeHitBox);

	// skill hit box
	auto skillObject = Scene::getCurrentScene()->AddGameObject(playerObj);
	skillObject->GetTransform()->position = Vector2d(85, 65);
	auto skillDispatcher = skillObject->AddComponent<BoxCollider2D>();
	skillDispatcher->SetHeight(270);
	skillDispatcher->SetWidth(200);
	skillDispatcher->GetGameObject()->AddComponent<CollisionEventDispatcher>()->target = playerComp;

	playerComp->playerSkillHitBox = skillDispatcher;
	DebugObject::CreateDebugRectImage(skillObject, 200, 270, D2D1::ColorF::Green);

	//animation
	player->GetTransform()->SetWorldPosition(pos);
	auto legObject = Scene::getCurrentScene()->AddGameObject(playerObj);
	auto torsoObject = Scene::getCurrentScene()->AddGameObject(playerObj);
	torsoObject->GetTransform()->position = Vector2d(0, 45);
	legObject->GetTransform()->position = Vector2d(0, 45);
	auto anim2 = legObject->AddComponent<D2DAnimatedSprite>();
	auto anim = torsoObject->AddComponent<D2DAnimatedSprite>();
	anim->SetHeight(256);
	anim->SetWidth(256);
	anim2->SetHeight(256);
	anim2->SetWidth(256);

	playerComp->GetInstance()->damagedAnimObj = Scene::getCurrentScene()->AddGameObject(player);
	auto damagedAnim = playerComp->GetInstance()->damagedAnimObj->AddComponent<D2DAnimatedSprite>();
	damagedAnim->SetWidth(128);
	damagedAnim->SetHeight(128);

	playerComp->Initialize(anim, anim2, damagedAnim);

	//drone
	auto drone = Drone::CreateDrone(Vector3d::zero);
	drone->SetParent(player);
	drone->GetTransform()->position = Vector3d(0, 200, 0);
	playerComp->pdrone = drone->GetComponent<Drone>();

	//debugging
	int* hpPointer = &playerComp->hp;
	DebugObject::CreateDebugText(player,
		[=]() {
			return to_wstring(*hpPointer);
		}, Vector2d(0, 200), 20, D2D1::ColorF::Blue);
	DebugObject::CreateDebugText(player, []() {
		return to_wstring(instance->combo);
		}, Vector2d(0, 400), 20, D2D1::ColorF::Red);
	DebugObject::CreateDebugText(player, []() {
		return to_wstring(instance->gauge);
		}, Vector2d(0, 300), 20, D2D1::ColorF::Yellow);
	DebugObject::CreateDebugText(player, []() {
		return to_wstring((int)instance->state);
		}, Vector2d(-300, 200), 20, D2D1::ColorF::Yellow);
	DebugObject::CreateDebugText(player, []() {
		return to_wstring(instance->skillCount);
		}, Vector2d(500, 300), 20, D2D1::ColorF::Yellow);

	for (int i = 0; i < 5; i++)
	{
		auto skillRushObject = player->AddGameObject();
		playerComp->skillRushTorsos[i] = skillRushObject->AddComponent<D2DAnimatedSprite>();
		playerComp->skillRushLegs[i] = skillRushObject->AddComponent<D2DAnimatedSprite>();

		playerComp->skillRushTorsos[i]->LoadAnimationFromFile(L"sprites/player/torso/skill");
		playerComp->skillRushLegs[i]->LoadAnimationFromFile(L"sprites/player/legs/dash");
		playerComp->skillRushTorsos[i]->SetIsRepeating(false);
		playerComp->skillRushLegs[i]->SetIsRepeating(false);

		playerComp->skillRushTorsos[i]->SetHeight(256);
		playerComp->skillRushTorsos[i]->SetWidth(256);
		playerComp->skillRushLegs[i]->SetHeight(256);
		playerComp->skillRushLegs[i]->SetWidth(256);

		playerComp->skillRushTorsos[i]->color.a = 0;
		playerComp->skillRushLegs[i]->color.a = 0;
	}

	playerComp->ResetPlayerStatus();
	return player;
}

void Player::ResetPlayerStatus()
{
	steppedGrounds.clear();
	closeEnemies.clear();
	closeEnemiesSetForSkill.clear();
	projectilesToParry.clear();
	skipFrame = true;
}
