#pragma once
#include "YunutyEngine.h"
#include "Timer.h"
#include "Player.h"

enum class DroneState
{
	IDLE,
	FLY,
	ATTACK,
	DEATH
};

class Drone : public Component
{
public:
	static GameObject* CreateDrone(Vector3d pos);
	Drone* GetInstance();
	Drone();

protected:
	virtual void Update() override;
	virtual void Start() override;
private:
	static Drone* instance;
	Vector3d speed;
	Timer durationTimer;
	Timer attackTimer;
	Timer attackCoolDownTimer;
	Timer idleTimer;
	Timer deathAnimTimer;
	D2DAnimatedSprite* animatedSprite;
	Vector3d enemyPos;
	Vector3d dronePos;

	Player* pinstance = Player::GetInstance();

	bool justAttack;
	bool attackJustchanged;
	double damage;

	void Initialize(D2DAnimatedSprite* animatedSprite);
};