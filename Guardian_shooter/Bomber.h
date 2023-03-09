#pragma once
#include "Threat.h"
#include "unordered_set"

class Ground;
class DamageEffect;

enum class BomberState
{
	IDLE,
	ATTACK,
	DEATH
};

class Bomber : public Threat
{
public:
	Bomber();
	static GameObject* CreateBomber(Vector3d pos);
	void Initialize(D2DAnimatedSprite* anim);
	virtual void Damage(double damage) override;
	void Reset();

protected:
	virtual void OnDeath() override;
	virtual void Update() override;
	virtual void OnCollisionEnter2D(const Collision2D& collision) override;
	virtual void OnCollisionExit2D(const Collision2D& collision) override;

private:
	BoxCollider2D* bomberHitBox;
	D2DAnimatedSprite* bomberAnimation;
	DamageEffect* effect = nullptr;
	Vector3d speed;
	BomberState bomberState = BomberState::IDLE;
	Timer idleTimer;
	Timer attackCoolDownTimer;
	Timer deathAnimTimer;
	Timer damageEffect;

	const double gravity = 98.0;
	double sight = 800.0;
	const double attackRange = 800.0;
	const double damage = 1.0;

	char horizontalView = -1; //1 = right, -1 = left

	bool idleFinished = true;
	bool attackFinished = true;
	bool isAttackReady = false;
	bool isBombShooted = false;
	bool isGroundStepped = false;
	bool isDying = false;
	std::unordered_set<Ground*> steppedGrounds;
};