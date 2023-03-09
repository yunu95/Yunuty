#pragma once
#include "Threat.h"
#include "unordered_set"

class Ground;
class DamageEffect;

enum class ShooterOneShotState
{
	IDLE,
	PATROL,
	CHASE,
	FALL,
	ATTACK,
	DEATH
};

class ShooterOneShot : public Threat
{
public:
	ShooterOneShot();
	static GameObject* CreateShooterOneShot(Vector3d pos);
	void Initialize(D2DAnimatedSprite* anim);
	virtual void Damage(double damage) override;
	void Reset();

protected:
	virtual void OnDeath() override;
	virtual void Update() override;
	virtual void OnCollisionEnter2D(const Collision2D& collision) override;
	virtual void OnCollisionExit2D(const Collision2D& collision) override;

private:
	BoxCollider2D* shooterHitBox;
	D2DAnimatedSprite* shooterAnimation;
	DamageEffect* effect = nullptr;
	Vector3d speed;
	ShooterOneShotState shooterState = ShooterOneShotState::FALL;
	Timer idleTimer;
	Timer patrolTimer;
	Timer attackCoolDownTimer;
	Timer deathAnimTimer;
	Timer damageEffect;

	const double gravity = 98.0;
	double sight = 700.0;
	const double attackRange = 600.0;
	const double damage = 1.0;

	char horizontalView = -1; //1 = right, -1 = left

	bool idleFinished = true;
	bool walkFinished = true;
	bool attackFinished = true;
	bool isGroundStepped = false;
	bool isChasing = false;
	bool isDying = false;
	std::unordered_set<Ground*> steppedGrounds;
};