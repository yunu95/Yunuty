#include "Boss.h"
#include "ProjectilePool.h"
#include "ThreatPool.h"
#include "Player.h"
#include "DebugObject.h"
#include "CollisionEventDispatcher.h"
#include "Guardian_shooter.h"
#include <random>

Boss* Boss::instance = nullptr;
std::random_device randDevice;
std::mt19937 gen(randDevice());

Boss::Boss()
{
	instance = this;
	idleTimer.duration = 1.0;
	idleTimer.onExpiration = [this]()
	{
		idleFinished = true;
	};
	earthquakeReadyAnimTimer.duration = 2.0;
	earthquakeReadyAnimTimer.onExpiration = [this]()
	{
		isEarthquakeReady = true;
	};
	earthquakeAtkAnimTimer.duration = 3.0;
	earthquakeAtkAnimTimer.onExpiration = [this]()
	{
		attackFinished = true;
	};
	handColliderReadyTimer.duration = 2.0;
	handColliderReadyTimer.onExpiration = [this]()
	{
		isHandColliderReady = true;
	};
	handColliderOnTimer.duration = 0.1;
	handColliderOnTimer.onExpiration = [this]()
	{
		isEarthquakeMode = true;
		SoundSystem::PlaySoundfile("sounds/boss/Boss_pt1rumbling.mp3");
		// 팔 콜라이더 꺼주기
		bossHandObject->GetComponent<BoxCollider2D>()->GetGameObject()->SetSelfActive(false);
		// 지면에 설치한 콜라이더 꺼주기
		groundObject->GetComponent<BoxCollider2D>()->GetGameObject()->SetSelfActive(false);
	};
	earthquakeDelayTimer.duration = 0.3;
	earthquakeDelayTimer.onExpiration = [this]()
	{
		earthquakeEffectTimer.Start();
		effectHitBoxOnTimer.Start();

		earthquakeEffectAnim->Play();
		// effect anim on
		eqEffectAnimObj->SetSelfActive(true);
	};
	earthquakeEffectTimer.duration = 0.4;
	earthquakeEffectTimer.onExpiration = [this]()
	{
		++earthquakeAtkOrder;
		if (earthquakeAtkOrder >= 4)
		{
			earthquakeAtkOrder = 0;
			isEarthquakeMode = false;
		}
		// effect collider off
		eqEffectColliderObj->SetSelfActive(false);
		// effect anim off
		eqEffectAnimObj->SetSelfActive(false);
		earthquakeEffectAnim->GetGameObject()->SetSelfActive(false);
	};
	effectHitBoxOnTimer.duration = 0.15;
	effectHitBoxOnTimer.onExpiration = [this]()
	{
		// effect collider on
		eqEffectColliderObj->SetSelfActive(true);
	};
	eqCameraShakeTimer.duration = 0.75;
	eqCameraShakeTimer.onUpdate = [](double progress)
	{
		Vector3d cameraPos = GSCamera::GetInstance()->GetTransform()->GetWorldPosition();
		std::uniform_real_distribution<double> randPos(-12, 12);
		double randX = randPos(gen);
		double randY = randPos(gen);
		GSCamera::GetInstance()->GetTransform()->SetWorldPosition(Vector3d(cameraPos.x + randX, cameraPos.y + randY, 0));
	};
	eqCameraShakeTimer.onExpiration = [this]()
	{
		// 카메라 원래 위치로 되돌리기
		GSCamera::GetInstance()->GetTransform()->SetWorldPosition(originCameraPos);
	};

	missileAnimTimer.duration = 4.0;
	missileAnimTimer.onExpiration = [this]()
	{			
		attackFinished = true;					
	};
	missileReadyTimer.duration = 0.4;
	missileReadyTimer.onExpiration = [this]()
	{
		isMissileReady = true;
	};
	mslCameraShakeTimer.duration = 0.15;
	mslCameraShakeTimer.onUpdate = [](double progress)
	{
		Vector3d cameraPos = GSCamera::GetInstance()->GetTransform()->GetWorldPosition();
		std::uniform_real_distribution<double> randPos(-1, 1);
		double randX = randPos(gen);
		GSCamera::GetInstance()->GetTransform()->SetWorldPosition(Vector3d(cameraPos.x + randX, cameraPos.y + 3, 0));
	};
	mslCameraShakeTimer.onExpiration = [this]()
	{
		// 카메라 원래 위치로 되돌리기
		GSCamera::GetInstance()->GetTransform()->SetWorldPosition(originCameraPos);
	};

	laserReadyAnimTimer.duration = 2.0;
	laserReadyAnimTimer.onExpiration = [this]()
	{
		isLaserReady = true;
	};
	laserLastingTimer.duration = 1.0;
	laserLastingTimer.onExpiration = [this]()
	{		
		isLaserShooted = true;
	};
	laserModeFinishTimer.duration = 1.0;
	laserModeFinishTimer.onExpiration = [this]()
	{
		attackFinished = true;
	};
	laserCameraShakeTimer.duration = 1.35;
	laserCameraShakeTimer.onUpdate = [](double progress)
	{
		Vector3d cameraPos = GSCamera::GetInstance()->GetTransform()->GetWorldPosition();
		std::uniform_real_distribution<double> randPos(-5, 5);
		double randX = randPos(gen);
		double randY = randPos(gen);
		GSCamera::GetInstance()->GetTransform()->SetWorldPosition(Vector3d(cameraPos.x + randX, cameraPos.y + randY, 0));
	};
	laserCameraShakeTimer.onExpiration = [this]()
	{
		GSCamera::GetInstance()->GetTransform()->SetWorldPosition(originCameraPos);
	};

	barrageAnimTimer.duration = 1.5;
	barrageAnimTimer.onUpdate = [](double progress)
	{
		Vector3d cameraPos = GSCamera::GetInstance()->GetTransform()->GetWorldPosition();
		std::uniform_real_distribution<double> randPos(-2, 2);
		double randY = randPos(gen);
		GSCamera::GetInstance()->GetTransform()->SetWorldPosition(Vector3d(cameraPos.x + 2.0 * (1 - progress), cameraPos.y + randY, 0));
	};
	barrageAnimTimer.onExpiration = [this]()
	{
		isBarrageAtkReady = true;
		bgExplosionShakeTimer.Start();
	};
	bgExplosionShakeTimer.duration = 0.2;
	bgExplosionShakeTimer.onUpdate = [](double progress)
	{
		Vector3d cameraPos = GSCamera::GetInstance()->GetTransform()->GetWorldPosition();
		GSCamera::GetInstance()->GetTransform()->SetWorldPosition(Vector3d(cameraPos.x - 3, cameraPos.y + 3, 0));
	};
	bgExplosionShakeTimer.onExpiration = [this]()
	{
		GSCamera::GetInstance()->GetTransform()->SetWorldPosition(originCameraPos);
	};
	barrageDelayTimer.duration = 1.0;
	barrageDelayTimer.onExpiration = [this]()
	{
		isBarrageAtkReady = true;
		bgExplosionShakeTimer.Start();
	};
	barrageFinAnimTimer.duration = 0.5;
	barrageFinAnimTimer.onExpiration = [this]()
	{
		attackFinished = true;
		isAllBulletsShooted = false;
	};

	hitEffectTimer.duration = 0.7;
	hitEffectTimer.onUpdate = [this](double progress)
	{
		if (bossAnimation->color.a <= 0.90)
		{
			bossAnimation->color.a += progress * 0.8;			
		}
		else if (bossAnimation->color.a >= 0.99)
		{
			bossAnimation->color.a -= progress * 0.8;
		}
	};
	hitEffectTimer.onExpiration = [this]()
	{		
		bossAnimation->color.a = 1.0;
	};

	deathAnimTimer.duration = 10.0;
	deathAnimTimer.onExpiration = [this]()
	{
		isDying = true;
	};

}

GameObject* Boss::CreateBoss(Vector3d pos)
{
	// ThreatPool 없으면 생성.
	if (ThreatPool::GetInstance() == nullptr)
	{
		GameObject* threatPool = Scene::getCurrentScene()->AddGameObject();
		threatPool->AddComponent<ThreatPool>();
	}
	if (instance)
	{
		SetBossPos(pos);
		instance->ResetBossStatus();
		instance->GetGameObject()->SetSelfActive(true);
		return instance->GetGameObject();
	}

	auto boss = Scene::getCurrentScene()->AddGameObject();
	boss->GetTransform()->SetWorldPosition(pos);
	// hitbox
	boss->AddComponent<Boss>()->bossHitBox = boss->AddComponent<BoxCollider2D>();
	boss->GetComponent<BoxCollider2D>()->SetWidth(180);
	boss->GetComponent<BoxCollider2D>()->SetHeight(750);
	DebugObject::CreateColliderImage(instance->bossHitBox);
	DebugObject::CreateDebugText(boss, [boss]() {
		return to_wstring((int)boss->GetComponent<Boss>()->bossState);
		}, Vector2d(0, 200), 20, D2D1::ColorF::Blue);

	// arm collider
	boss->GetComponent<Boss>()->GetInstance()->bossHandObject = Scene::getCurrentScene()->AddGameObject(boss);
	GameObject* childObject = boss->GetComponent<Boss>()->GetInstance()->bossHandObject;
	childObject->GetTransform()->position = Vector2d(-200, 0);
	auto armDispatcher = childObject->AddComponent<BoxCollider2D>();
	armDispatcher->SetHeight(100);
	armDispatcher->SetWidth(200);
	armDispatcher->GetGameObject()->AddComponent<CollisionEventDispatcher>()->target = boss->GetComponent<Boss>();

	boss->GetComponent<Boss>()->bossArmHitBox = armDispatcher;
	DebugObject::CreateColliderImage(instance->bossArmHitBox, D2D1::ColorF::Green);
	childObject->SetSelfActive(false);

	// earthquake effect collider
	boss->GetComponent<Boss>()->GetInstance()->eqEffectColliderObj = Scene::getCurrentScene()->AddGameObject(boss);
	GameObject* effectColObject = boss->GetComponent<Boss>()->GetInstance()->eqEffectColliderObj;
	auto effectDispatcher = effectColObject->AddComponent<BoxCollider2D>();
	effectDispatcher->SetHeight(180);
	effectDispatcher->SetWidth(220);
	effectDispatcher->GetGameObject()->AddComponent<CollisionEventDispatcher>()->target = boss->GetComponent<Boss>();

	boss->GetComponent<Boss>()->earthquakeEffectHitBox = effectDispatcher;
	DebugObject::CreateColliderImage(instance->earthquakeEffectHitBox, D2D1::ColorF::Cyan);
	effectColObject->SetSelfActive(false);

	// earthquake effect anim
	boss->GetComponent<Boss>()->GetInstance()->eqEffectAnimObj = Scene::getCurrentScene()->AddGameObject(boss);
	auto effectAnim = boss->GetComponent<Boss>()->GetInstance()->eqEffectAnimObj->AddComponent<D2DAnimatedSprite>();
	effectAnim->SetWidth(718);
	effectAnim->SetHeight(512);
	boss->GetComponent<Boss>()->GetInstance()->eqEffectAnimObj->SetSelfActive(false);

	// ground collider for earthquake skill
	boss->GetComponent<Boss>()->GetInstance()->groundObject = Scene::getCurrentScene()->AddGameObject(boss);
	GameObject* groundObj = boss->GetComponent<Boss>()->GetInstance()->groundObject;
	groundObj->GetTransform()->position = Vector2d(-700, -370);
	auto groundDispatcher = groundObj->AddComponent<BoxCollider2D>();
	groundDispatcher->SetHeight(100);
	groundDispatcher->SetWidth(1500);
	groundDispatcher->GetGameObject()->AddComponent<CollisionEventDispatcher>()->target = boss->GetComponent<Boss>();
	boss->GetComponent<Boss>()->groundHitBox = groundDispatcher;
	DebugObject::CreateColliderImage(instance->groundHitBox, D2D1::ColorF::Green);
	groundObj->GetComponent<BoxCollider2D>()->GetGameObject()->SetSelfActive(false);

	// bullet start point
	boss->GetComponent<Boss>()->GetInstance()->bulletStartPoint = Scene::getCurrentScene()->AddGameObject(boss);
	GameObject* bulletPointObject = boss->GetComponent<Boss>()->GetInstance()->bulletStartPoint;
	bulletPointObject->GetTransform()->position = Vector2d(-323, 195);
	bulletPointObject->AddComponent<BoxCollider2D>();
	bulletPointObject->GetComponent<BoxCollider2D>()->SetWidth(100);
	bulletPointObject->GetComponent<BoxCollider2D>()->SetHeight(100);
	DebugObject::CreateColliderImage(bulletPointObject->GetComponent<BoxCollider2D>(), D2D1::ColorF::Blue);
	// missile start point left
	boss->GetComponent<Boss>()->GetInstance()->missileStartPointLeft = Scene::getCurrentScene()->AddGameObject(boss);
	GameObject* missilePointObjectL = boss->GetComponent<Boss>()->GetInstance()->missileStartPointLeft;
	missilePointObjectL->GetTransform()->position = Vector2d(-100, 250);
	missilePointObjectL->AddComponent<BoxCollider2D>();
	missilePointObjectL->GetComponent<BoxCollider2D>()->SetWidth(100);
	missilePointObjectL->GetComponent<BoxCollider2D>()->SetHeight(100);
	DebugObject::CreateColliderImage(missilePointObjectL->GetComponent<BoxCollider2D>(), D2D1::ColorF::Red);
	// missile start point right
	boss->GetComponent<Boss>()->GetInstance()->missileStartPointRight = Scene::getCurrentScene()->AddGameObject(boss);
	GameObject* missilePointObjectR = boss->GetComponent<Boss>()->GetInstance()->missileStartPointRight;
	missilePointObjectR->GetTransform()->position = Vector2d(40, 270);
	missilePointObjectR->AddComponent<BoxCollider2D>();
	missilePointObjectR->GetComponent<BoxCollider2D>()->SetWidth(100);
	missilePointObjectR->GetComponent<BoxCollider2D>()->SetHeight(100);
	DebugObject::CreateColliderImage(missilePointObjectR->GetComponent<BoxCollider2D>(), D2D1::ColorF::Red);

	// laser anim
	boss->GetComponent<Boss>()->GetInstance()->laserObject = Scene::getCurrentScene()->AddGameObject(boss);
	GameObject* laserObj = boss->GetComponent<Boss>()->GetInstance()->laserObject;
	laserObj->GetTransform()->position = Vector2d(-1230, 250);
	auto laserAnim = laserObj->AddComponent<D2DAnimatedSprite>();
	laserAnim->SetWidth(120);
	laserAnim->SetHeight(2000);
	laserAnim->GetTransform()->SetWorldRotation(Vector3d(0, 0, -90));

	// laser collider
	auto laserDispatcher = laserObj->AddComponent<BoxCollider2D>();
	laserDispatcher->SetWidth(50);
	laserDispatcher->SetHeight(2000);
	laserDispatcher->GetGameObject()->AddComponent<CollisionEventDispatcher>()->target = boss->GetComponent<Boss>();
	boss->GetComponent<Boss>()->laserHitBox = laserDispatcher;
	DebugObject::CreateColliderImage(instance->laserHitBox, D2D1::ColorF::Pink);
	laserObj->GetComponent<BoxCollider2D>()->GetGameObject()->SetSelfActive(false);

	//boss animation
	auto animObject = Scene::getCurrentScene()->AddGameObject(boss);
	auto anim = animObject->AddComponent<D2DAnimatedSprite>();
	anim->GetTransform()->position = Vector2d(-220, 85);
	anim->SetWidth(1024);
	anim->SetHeight(1024);

	boss->GetComponent<Boss>()->Initialize(anim, effectAnim, laserAnim);

	return boss;
}

void Boss::Initialize(D2DAnimatedSprite* anim, D2DAnimatedSprite* effectAnim, D2DAnimatedSprite* laserAnimation)
{
	hp = maxHp;
	invincible = false;
	threatType = ThreatType::BOSS;
	idleTimer.Start();
	idleFinished = false;
	// boss anim
	bossAnimation = anim;
	bossAnimation->SetIsRepeating(true);
	bossAnimation->LoadAnimationFromFile(L"sprites/enemy/boss/idle");
	// eqrthquake effect anim
	earthquakeEffectAnim = effectAnim;
	earthquakeEffectAnim->SetIsRepeating(false);
	earthquakeEffectAnim->LoadAnimationFromFile(L"sprites/enemy/boss/earthquake/effect");
	// laser anim
	laserAnim = laserAnimation;
	laserAnim->SetIsRepeating(false);
	laserAnim->LoadAnimationFromFile(L"sprites/projectile/laser/forward");
	laserAnim->GetGameObject()->SetSelfActive(false);
}

void Boss::Damage(double damage)
{
	if (bossState == BossState::DEATH)
		return;
	Threat::Damage(damage);

	// 피격 효과 출력
	if (hitEffectTimer.isActive == false)
	{
		hitEffectTimer.Start();
		//bossAnimation->color.a = 0.8;
	}
}

void Boss::ResetBossStatus()
{
	idleTimer.Start();
	idleFinished = false;
	attackFinished = true;
	isEarthquakeReady = false;
	isEarthquakeMode = false;
	isHandColliderReady = false;
	earthquakeAtkOrder = 0;

	isMissileReady = true;	
	missileCount = 0;

	isLaserReady = false;
	bool isLaserMode = false;
	bool isLaserShooted = false;

	isBarrageMode = false;
	isBarrageAtkReady = false;
	bool isAllBulletsShooted = false;
	barrageAtkCount = 0;

	bossState = BossState::IDLE;
	hp = maxHp;

	timeScaleOnDeath = 0.5;
}

size_t Boss::RandomPattern()
{
	std::uniform_int_distribution<size_t> randomNum(1, 4);
	return randomNum(gen);
}

void Boss::OnDeath()
{
	bossState = BossState::DEATH;
	// 죽는 애니메이션 출력용 타이머 켜주기
	deathAnimTimer.Start();
	// 사운드 켜주기
	SoundSystem::PlaySoundfile("sounds/boss/Boss_dying2.wav");
	// 애니메이션 불러오기
	bossAnimation->SetIsRepeating(false);
	bossAnimation->LoadAnimationFromFile(L"sprites/enemy/boss/death");
	// 타임스케일 느리게
	Time::SetTimeScale(timeScaleOnDeath);
}

void Boss::Update()
{
	bool stateJustChanged = false;

	// state change
	switch (bossState)
	{
		case BossState::IDLE:
		{
			// 타이머 다 되면 공격 상태로 변경
			if (idleFinished)
			{
				if (attackFinished)
				{
					if (Player::GetInstance()->GetMovementState() != PlayerMovementState::DEATH)
					{
						bossState = (BossState)RandomPattern();
						//bossState = BossState::MISSILE;
						stateJustChanged = true;
					}
					
				}
			}
		}
		break;
		case BossState::EARTHQUAKE:
			// 타이머 다 되면 IDLE로 변경
			if (attackFinished)
			{
				bossState = BossState::IDLE;
				stateJustChanged = true;
			}
			break;
		case BossState::MISSILE:
			// 패턴 끝나면 IDLE로 변경
			if (attackFinished)
			{
				missileCount = 0;
				bossState = BossState::IDLE;
				stateJustChanged = true;
			}
			break;
		case BossState::LASER:
			// 타이머 다 되면 IDLE로 변경
			if (attackFinished)
			{
				bossState = BossState::IDLE;
				stateJustChanged = true;
			}
			break;
		case BossState::BARRAGE:
			// 타이머 다 되면 IDLE로 변경
			if (attackFinished)
			{
				bossState = BossState::IDLE;
				stateJustChanged = true;
			}
			break;
		case BossState::DEATH:
			break;
		default:
			break;
	}

	switch (bossState)
	{
		case BossState::IDLE:
		{
			if (stateJustChanged)
			{
				idleTimer.Start();
				idleFinished = false;
				bossAnimation->SetIsRepeating(true);
				bossAnimation->LoadAnimationFromFile(L"sprites/enemy/boss/idle");
			}
			if (!groundStepped)
			{
				speed.y += -9.8 * 20 * Time::GetDeltaTime();
				GetTransform()->SetWorldPosition(GetTransform()->GetWorldPosition() + speed * Time::GetDeltaTime());
			}
		}
		break;
		case BossState::EARTHQUAKE:
		{
			if (stateJustChanged)
			{
				earthquakeReadyAnimTimer.Start();
				handColliderReadyTimer.Start();
				groundObject->GetTransform()->position = Vector2d(-700, -360);
				originCameraPos = GSCamera::GetInstance()->GetTransform()->GetWorldPosition();
				attackFinished = false;
				SoundSystem::PlaySoundfile("sounds/boss/Boss_pt1hup.mp3");
				bossAnimation->SetIsRepeating(false);
				bossAnimation->LoadAnimationFromFile(L"sprites/enemy/boss/earthquake/ready");
			}
			if (isEarthquakeReady)
			{
				isEarthquakeReady = false;
				earthquakeAtkAnimTimer.Start();
				bossAnimation->SetIsRepeating(false);
				bossAnimation->LoadAnimationFromFile(L"sprites/enemy/boss/earthquake/attack");
			}
			if (isHandColliderReady)
			{
				isHandColliderReady = false;
				handColliderOnTimer.Start();
				eqCameraShakeTimer.Start();
				// 지면 내리치는 사운드
				SoundSystem::PlaySoundfile("sounds/boss/Boss_pt1smashground.mp3");
				handColliderPos = Vector3d(-320, -320, 0);
				bossHandObject->GetTransform()->position = handColliderPos;
				bossHandObject->GetComponent<BoxCollider2D>()->GetGameObject()->SetSelfActive(true);
				// 지면에 설치한 콜라이더 켜주기
				groundObject->GetComponent<BoxCollider2D>()->GetGameObject()->SetSelfActive(true);
			}
			if (isEarthquakeMode)
			{
				if (!earthquakeDelayTimer.isActive && !earthquakeEffectTimer.isActive)
					earthquakeDelayTimer.Start();
				switch (earthquakeAtkOrder)
				{
					case 0:
						eqEffectColliderObj->GetTransform()->position = Vector3d(-580, -300, 0);
						eqEffectAnimObj->GetTransform()->position = Vector3d(-580, -100, 0);
						break;
					case 1:
						eqEffectColliderObj->GetTransform()->position = Vector3d(-840, -300, 0);
						eqEffectAnimObj->GetTransform()->position = Vector3d(-840, -100, 0);
						break;
					case 2:
						eqEffectColliderObj->GetTransform()->position = Vector3d(-1100, -300, 0);
						eqEffectAnimObj->GetTransform()->position = Vector3d(-1100, -100, 0);
						break;
					case 3:
						eqEffectColliderObj->GetTransform()->position = Vector3d(-1360, -300, 0);
						eqEffectAnimObj->GetTransform()->position = Vector3d(-1360, -100, 0);
						break;
					default:
						break;
				}
			}
		}
		break;
		case BossState::MISSILE:
		{
			if (stateJustChanged)
			{
				attackFinished = false;				
				isMissileReady = true;
				missileAnimTimer.Start();
				originCameraPos = GSCamera::GetInstance()->GetTransform()->GetWorldPosition();
			}
			if (isMissileReady)
			{	
				if (missileCount < 8)
				{
					mslCameraShakeTimer.Start();
					Vector3d shootPos = Vector3d(0, 0, 0);
					if (missileCount % 2 == 0)
					{
						shootPos = missileStartPointLeft->GetTransform()->GetWorldPosition();
						bossAnimation->SetIsRepeating(false);
						bossAnimation->LoadAnimationFromFile(L"sprites/enemy/boss/missile/left");
						bossAnimation->Start();
					}
					else
					{
						shootPos = missileStartPointRight->GetTransform()->GetWorldPosition();
						bossAnimation->SetIsRepeating(false);
						bossAnimation->LoadAnimationFromFile(L"sprites/enemy/boss/missile/right");
						bossAnimation->Start();
					}
					// 풀에서 미사일 소환
					SoundSystem::PlaySoundfile("sounds/boss/Boss_pt2lauchershoot.mp3");
					Vector3d bulletSpeed = Vector3d(-400.f, 0, 0);
					ProjectilePool::GetInstance()->SummonProjectile(shootPos, bulletSpeed, ProjectileType::MISSILE, missileDamage, false);
					++missileCount;
					isMissileReady = false;
					missileReadyTimer.Start();
				}
			}
		}
		break;
		case BossState::LASER:
		{
			if (stateJustChanged)
			{
				attackFinished = false;
				isLaserMode = true;
				isLaserShooted = false;
				originCameraPos = GSCamera::GetInstance()->GetTransform()->GetWorldPosition();
				laserReadyAnimTimer.Start();
				SoundSystem::PlaySoundfile("sounds/boss/Boss_pt3LaserCharging.mp3");
				bossAnimation->SetIsRepeating(false);
				bossAnimation->LoadAnimationFromFile(L"sprites/enemy/boss/laser/ready");
				bossAnimation->Start();
			}
			if (isLaserMode)
			{
				if (isLaserReady)
				{
					laserLastingTimer.Start();
					laserCameraShakeTimer.Start();
					laserAnim->GetGameObject()->SetSelfActive(true);
					laserHitBox->GetGameObject()->SetSelfActive(true);
					laserAnim->Start();
					SoundSystem::PlaySoundfile("sounds/boss/Boss_pt3Lazer.wav");
					bossAnimation->LoadAnimationFromFile(L"sprites/enemy/boss/laser/attack");
					bossAnimation->Start();
					isLaserReady = false;
				}
				if (isLaserShooted)
				{
					isLaserMode = false;
					laserModeFinishTimer.Start();
					laserAnim->GetGameObject()->SetSelfActive(false);
					laserHitBox->GetGameObject()->SetSelfActive(false);
					bossAnimation->LoadAnimationFromFile(L"sprites/enemy/boss/laser/finish");
					bossAnimation->Start();
				}
			}
		}
		break;
		case BossState::BARRAGE:
		{
			if (stateJustChanged)
			{
				attackFinished = false;
				isBarrageMode = true;
				originCameraPos = GSCamera::GetInstance()->GetTransform()->GetWorldPosition();
				SoundSystem::PlaySoundfile("sounds/boss/Boss_pt4charging.mp3");
				barrageAnimTimer.Start();
				bossAnimation->SetIsRepeating(false);
				bossAnimation->LoadAnimationFromFile(L"sprites/enemy/boss/barrage/attack");
			}
			if (isBarrageMode)
			{
				if (isBarrageAtkReady)
				{
					switch (barrageAtkCount)
					{
						case 0:
						{
							isBarrageAtkReady = false;
							++barrageAtkCount;
							for (size_t i = 0; i < 13; ++i)
							{
								Vector3d bulletSpeed = Vector3d(2.9 * (0 + 100) * cos((130 + i * 180 / 12) * 3.14 / 180), 3.1 * (0 + 100) * sin((130 + i * 180 / 12) * 3.14 / 180), 0);
								ProjectilePool::GetInstance()->SummonProjectile(bulletStartPoint->GetTransform()->GetWorldPosition(), bulletSpeed, ProjectileType::BOSSBULLET, bulletDamage, false);
							}
							SoundSystem::PlaySoundfile("sounds/boss/Boss_pt4chargingAftermath.wav");
							barrageDelayTimer.Start();
						}
						break;
						case 1:
						{
							isBarrageAtkReady = false;
							++barrageAtkCount;
							for (size_t i = 0; i < 12; ++i)
							{
								Vector3d bulletSpeed = Vector3d(3.0 * (0 + 110) * cos((140 + i * 180 / 11) * 3.14 / 180), 3.2 * (0 + 110) * sin((120 + i * 180 / 11) * 3.14 / 180), 0);
								ProjectilePool::GetInstance()->SummonProjectile(bulletStartPoint->GetTransform()->GetWorldPosition(), bulletSpeed, ProjectileType::BOSSBULLET, bulletDamage, false);
							}
							barrageDelayTimer.Start();
						}
						break;
						case 2:
						{
							isBarrageAtkReady = false;
							++barrageAtkCount;
							for (size_t i = 0; i < 11; ++i)
							{
								Vector3d bulletSpeed = Vector3d(3.1 * (0 + 110) * cos((140 + i * 180 / 10) * 3.14 / 180), 3.3 * (0 + 110) * sin((110 + i * 180 / 10) * 3.14 / 180), 0);
								ProjectilePool::GetInstance()->SummonProjectile(bulletStartPoint->GetTransform()->GetWorldPosition(), bulletSpeed, ProjectileType::BOSSBULLET, bulletDamage, false);
							}
							barrageAtkCount = 0;
							isAllBulletsShooted = true;
						}
						break;
						default:
							break;
					}
				}
				if (isAllBulletsShooted)
				{
					isBarrageMode = false;
					barrageFinAnimTimer.Start();
					bossAnimation->SetIsRepeating(false);
					bossAnimation->LoadAnimationFromFile(L"sprites/enemy/boss/barrage/finish");
					bossAnimation->Start();
				}
			}
		}
		break;
		case BossState::DEATH:
			timeScaleOnDeath += 0.15 * Time::GetDeltaTimeUnscaled();
			if (timeScaleOnDeath >= 0.7)
			{
				timeScaleOnDeath = 0.7;
			}
			Time::SetTimeScale(timeScaleOnDeath);
			deathAnimTimer.Update();			
			if (isDying)
			{
				isDying = false;
				Time::SetTimeScale(1.0);
				/// 풀에 다시 넣어주기. 상태 초기화도 필요.
				ThreatPool::GetInstance()->Retrieve(this);
				ResetBossStatus();
			}
			break;
		default:
			break;
	}

	idleTimer.Update();

	earthquakeReadyAnimTimer.Update();
	earthquakeAtkAnimTimer.Update();
	handColliderReadyTimer.Update();
	handColliderOnTimer.Update();
	earthquakeDelayTimer.Update();
	earthquakeEffectTimer.Update();
	effectHitBoxOnTimer.Update();

	missileAnimTimer.Update();
	missileReadyTimer.Update();

	laserReadyAnimTimer.Update();
	laserLastingTimer.Update();
	laserModeFinishTimer.Update();
	laserCameraShakeTimer.Update();

	barrageAnimTimer.Update();
	barrageDelayTimer.Update();
	barrageFinAnimTimer.Update();
	bgExplosionShakeTimer.Update();

	eqCameraShakeTimer.Update();
	mslCameraShakeTimer.Update();

	hitEffectTimer.Update();
}

void Boss::OnCollisionEnter2D(const Collision2D& collision)
{
	if (collision.m_Collider == bossArmHitBox)
	{
		auto player = collision.m_OtherCollider->GetGameObject()->GetComponent<Player>();
		if (player != nullptr)
		{
			player->Damage();
		}
	}

	if (collision.m_Collider == earthquakeEffectHitBox)
	{
		auto player = collision.m_OtherCollider->GetGameObject()->GetComponent<Player>();
		if (player != nullptr)
		{
			player->Damage();
		}
	}

	if (collision.m_Collider == bossHitBox)
	{
		auto box = collision.m_OtherCollider->GetGameObject()->GetComponent<Ground>();
		if (box != nullptr && (
			(!box->isPenetrable && box->isSteppable()) ||
			(box->isPenetrable && box->isSteppable())))			
		{			
			groundStepped = true;			
		}
	}	
}

void Boss::OnCollisionStay2D(const Collision2D& collision)
{
	if (collision.m_Collider == laserHitBox)
	{
		auto player = collision.m_OtherCollider->GetGameObject()->GetComponent<Player>();
		if (player != nullptr)
		{
			player->Damage();
		}
	}

	if (collision.m_Collider == groundHitBox)
	{
		auto player = collision.m_OtherCollider->GetGameObject()->GetComponent<Player>();
		if (player != nullptr)
		{
			player->Damage();
		}
	}
}

void Boss::OnCollisionExit2D(const Collision2D& collision)
{
	if (collision.m_Collider == bossHitBox)
	{
		auto box = collision.m_OtherCollider->GetGameObject()->GetComponent<Ground>();
		if (box != nullptr && (
			(!box->isPenetrable && box->isSteppable()) ||
			(box->isPenetrable && box->isSteppable())))
		{
			groundStepped = false;
		}
	}
}