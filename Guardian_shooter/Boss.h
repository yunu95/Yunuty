#pragma once
#include "YunutyEngine.h"
#include "Threat.h"

namespace UI
{
	class UIManager;
}

enum class BossState
{
	IDLE,
	EARTHQUAKE,
	MISSILE,
	LASER,
	BARRAGE,
	DEATH,
};

class Boss : public Threat
{
	friend class UI::UIManager;
public:
	Boss();
	virtual ~Boss() { if (instance == this) instance = nullptr; }
	static Boss* GetInstance() { return instance; }
	static GameObject* CreateBoss(Vector3d pos);
	void Initialize(D2DAnimatedSprite* anim, D2DAnimatedSprite* effectAnim, D2DAnimatedSprite* laserAnimation);
	static Vector3d GetBossPos() { return instance->GetTransform()->GetWorldPosition(); }
	static void SetBossPos(Vector3d pos) { instance->GetTransform()->SetWorldPosition(pos); }
	virtual void Damage(double damage) override;
	void ResetBossStatus();

protected:
	virtual void OnDeath() override;
	virtual void Update() override;
	virtual void OnCollisionEnter2D(const Collision2D& collision) override;
	virtual void OnCollisionStay2D(const Collision2D& collision) override;
	virtual void OnCollisionExit2D(const Collision2D& collision) override;

private:
	static Boss* instance;
	GameObject* bossHandObject;
	GameObject* groundObject;
	GameObject* eqEffectColliderObj;
	GameObject* eqEffectAnimObj;
	GameObject* bulletStartPoint;
	GameObject* missileStartPointLeft;
	GameObject* missileStartPointRight;
	GameObject* laserObject;
	BoxCollider2D* bossHitBox;
	BoxCollider2D* bossArmHitBox;
	BoxCollider2D* groundHitBox;
	BoxCollider2D* earthquakeEffectHitBox;
	BoxCollider2D* laserHitBox;
	D2DAnimatedSprite* bossAnimation;
	D2DAnimatedSprite* earthquakeEffectAnim;
	D2DAnimatedSprite* laserAnim;
	Vector3d speed;
	BossState bossState = BossState::IDLE;

	Timer idleTimer;

	Timer earthquakeReadyAnimTimer;
	Timer earthquakeAtkAnimTimer;
	Timer handColliderReadyTimer;
	Timer handColliderOnTimer;
	Timer earthquakeDelayTimer;
	Timer earthquakeEffectTimer;
	Timer effectHitBoxOnTimer;
	Timer eqCameraShakeTimer;

	Timer missileAnimTimer;
	Timer missileReadyTimer;
	Timer mslCameraShakeTimer;

	Timer laserReadyAnimTimer;
	Timer laserLastingTimer;
	Timer laserModeFinishTimer;
	Timer laserCameraShakeTimer;

	Timer barrageAnimTimer;
	Timer barrageDelayTimer;
	Timer barrageFinAnimTimer;
	Timer bgExplosionShakeTimer;

	Timer hitEffectTimer;
	Timer deathAnimTimer;

	const double maxHp = 500.0;
	const double earthquakeDamage = 1.0;
	const double laserDamage = 1.0;
	const double missileDamage = 1.0;
	const double bulletDamage = 1.0;

	bool groundStepped = false;

	bool idleFinished = false;
	bool attackFinished = true;

	bool isEarthquakeReady = false;
	bool isEarthquakeMode = false;
	bool isHandColliderReady = false;
	size_t earthquakeAtkOrder = 0;
	Vector3d handColliderPos;

	bool isMissileReady = true;
	size_t missileCount = 0;

	bool isLaserReady = false;
	bool isLaserMode = false;
	bool isLaserShooted = false;

	bool isBarrageMode = false;
	bool isBarrageAtkReady = false;
	bool isAllBulletsShooted = false;
	size_t barrageAtkCount = 0;

	bool isDying = false;
	double timeScaleOnDeath = 0.4;

	Vector3d originCameraPos;

private:
	size_t RandomPattern();
	void Hit();
};