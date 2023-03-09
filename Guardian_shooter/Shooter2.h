#pragma once
#include "Threat.h"

class DamageEffect;

enum class Shooter2State
{
	IDLE,
	FALLING,
	WALKING,
	ATTACK,
	DEATH,
	PATROL
};

class Shooter2 : public Threat		// 산탄 공격 적군 유닛
{
private:

	D2DAnimatedSprite* anim;
	Vector3d speed;			// enemy의 이동속도

	DamageEffect* effect = nullptr;

	Timer traceTimer;
	Timer animTimer;
	Timer attackDelay;
	Timer damageEffect;

	double damage;
	double gravity;
	int bulletCnt;
	bool canAttack;			// 공격 가능 여부
	bool attackFinished;
	bool justLanded;
	bool justDamaged;
	bool justFallen;

	bool edge;

	bool stateJustChanged;	// 상태가 막 바뀌었는지 여부. update문에서 반복되지 않게 하기 위함
	bool isSummoned;		// 유닛이 pool을 거쳐 Summon되었는지 여부

	void changeState(Shooter2State changedState);

	void Buckshot(int bullets);
	double Radian(double degree);


private:
	void Initialize(D2DAnimatedSprite* animatedSprite);

public:
	void SetDirection();

	Shooter2State state;	// 적군의 상태. idle, walking 등

	static GameObject* CreateShooter2(Vector3d pos);	// 무조건 있어야 하는 함수

	virtual void Damage(double damage) override;

	virtual void OnDeath() override;

	virtual void Start() override;

	virtual void Update() override;

	virtual void OnCollisionEnter2D(const Collision2D& collision) override;

	virtual void OnCollisionExit2D(const Collision2D& collision) override;
};

