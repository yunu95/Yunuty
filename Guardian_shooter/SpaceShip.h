#pragma once
#include "Threat.h"

class Laser;
class DamageEffect;

enum class SpaceShipState
{
	IDLE,
	CHARGE,
	ATTACK,
	Death
};

class SpaceShip : public Threat
{
private:
	D2DAnimatedSprite* animatedSprite;
	DamageEffect* effect = nullptr;

	Timer laserLastTimer;
	Timer chargeStartTimer;
	Timer attackStartTimer;
	Timer animTimer;
	Timer damageEffect;

	SpaceShipState state;
	bool deathStart;
	bool justChanged;
	double damage = 1;
	void changeState(SpaceShipState tochangeState);
public:
	void Initialize(D2DAnimatedSprite* upperAnimatedSprite);
	virtual void Start() override;
	virtual void Update() override;
	virtual void OnDeath() override;
	virtual void Damage(double damage);
	virtual void OnCollisionExit2D(const Collision2D& collision)override;
	Laser* laser = nullptr;
	static GameObject* CreateSpaceShip(Vector3d pos);
};

