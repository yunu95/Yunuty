#pragma once
#include "Threat.h"

class DamageEffect;

enum class Shooter1State
{
	IDLE,
	FALLING,
	WALKING,
	ATTACK,
	DEATH
};

class Shooter1 : public Threat		// 점사 공격 적군 유닛
{
private:

	D2DAnimatedSprite* anim;
	Vector3d speed;			// enemy의 이동속도

	DamageEffect* effect = nullptr;

	Timer traceTimer;
	Timer attackDelay;
	Timer interrupted;
	Timer animTimer;
	Timer attackStop;		// attack -> walking
	Timer attackDelay2;
	Timer damageEffect;

	double damage;
	double gravity;
	int bulletCnt;

	int randHorRange;
	int randVerRange;

	bool canAttack;			// 공격 가능 여부
	bool attackVerable;		// 세로 공격 가능 여부
	bool attackHorable;		// 가로 공격 가능 여부
	bool attackFinished;
	bool isAttacking;
	bool isShootingUp;
	bool isShootingForward;


	bool justLanded;
	bool justDamaged;
	bool justFallen;
	bool attackStart;
	bool deathStart;

	bool stateJustChanged;	// 상태가 막 바뀌었는지 여부. update문에서 반복되지 않게 하기 위함
	bool isSummoned;		// 유닛이 pool을 거쳐 Summon되었는지 여부
	
	void changeState(Shooter1State changedState);

private:
	void Initialize(D2DAnimatedSprite* animatedSprite);

protected:

	Shooter1State state;	// 적군의 상태. idle, walking 등

	virtual void OnDeath() override;

	virtual void Start() override;

	virtual void Update() override;

	virtual void OnCollisionEnter2D(const Collision2D& collision) override;

	virtual void OnCollisionExit2D(const Collision2D& collision) override;

public:
	static GameObject* CreateShooter1(Vector3d pos);
	virtual void Damage(double damage) override;

};

