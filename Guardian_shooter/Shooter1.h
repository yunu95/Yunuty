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

class Shooter1 : public Threat		// ���� ���� ���� ����
{
private:

	D2DAnimatedSprite* anim;
	Vector3d speed;			// enemy�� �̵��ӵ�

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

	bool canAttack;			// ���� ���� ����
	bool attackVerable;		// ���� ���� ���� ����
	bool attackHorable;		// ���� ���� ���� ����
	bool attackFinished;
	bool isAttacking;
	bool isShootingUp;
	bool isShootingForward;


	bool justLanded;
	bool justDamaged;
	bool justFallen;
	bool attackStart;
	bool deathStart;

	bool stateJustChanged;	// ���°� �� �ٲ������ ����. update������ �ݺ����� �ʰ� �ϱ� ����
	bool isSummoned;		// ������ pool�� ���� Summon�Ǿ����� ����
	
	void changeState(Shooter1State changedState);

private:
	void Initialize(D2DAnimatedSprite* animatedSprite);

protected:

	Shooter1State state;	// ������ ����. idle, walking ��

	virtual void OnDeath() override;

	virtual void Start() override;

	virtual void Update() override;

	virtual void OnCollisionEnter2D(const Collision2D& collision) override;

	virtual void OnCollisionExit2D(const Collision2D& collision) override;

public:
	static GameObject* CreateShooter1(Vector3d pos);
	virtual void Damage(double damage) override;

};

