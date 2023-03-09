#pragma once
#include "Threat.h"
#include "Shooter3.h"

class ShootingLaser;
class DamageEffect;

enum class Shooter3State
{
	IDLE,
	FALLING,
	WALKING,
	CHARGE,
	ATTACK,
	DEATH
};

class Shooter3 : public Threat		// 레이저 공격 적군 유닛
{
private:

	ShootingLaser* laser = nullptr;
	DamageEffect* effect = nullptr;

	D2DAnimatedSprite* anim;
	Vector3d speed;			// enemy의 이동속도

	Timer traceTimer;
	Timer LaserLastTimer;
	Timer attackDelay;		// 레이저 공격의 간격
	Timer animTimer;		// 사망 모션
	Timer laserShoot;		// 초탄 타이머
	Timer chargeStartTimer;
	Timer attackStartTimer;
	Timer laserLastTimer;
	Timer damageEffect;

	double gravity;
	int bulletCnt;

	int attackStart;		// 0이면 초탄
	bool canAttack;			// 공격 가능 여부

	bool justLanded;
	bool justDamaged;
	bool justFallen;

	bool stateJustChanged;	// 상태가 막 바뀌었는지 여부. update문에서 반복되지 않게 하기 위함
	bool isSummoned;		// 유닛이 pool을 거쳐 Summon되었는지 여부

	void changeState(Shooter3State changedState);

private:
	void Initialize(D2DAnimatedSprite* animatedSprite);

protected:
	double damage;

public:

	Shooter3State state;	// 적군의 상태. idle, walking 등

	static GameObject* CreateShooter3(Vector3d pos);	// 무조건 있어야 하는 함수

	virtual void Damage(double damage) override;

	virtual void OnDeath() override;

	virtual void Start() override;

	virtual void Update() override;

	virtual void OnCollisionEnter2D(const Collision2D& collision) override;

	virtual void OnCollisionExit2D(const Collision2D& collision) override;
};

