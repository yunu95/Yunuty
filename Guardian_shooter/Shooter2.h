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

class Shooter2 : public Threat		// ��ź ���� ���� ����
{
private:

	D2DAnimatedSprite* anim;
	Vector3d speed;			// enemy�� �̵��ӵ�

	DamageEffect* effect = nullptr;

	Timer traceTimer;
	Timer animTimer;
	Timer attackDelay;
	Timer damageEffect;

	double damage;
	double gravity;
	int bulletCnt;
	bool canAttack;			// ���� ���� ����
	bool attackFinished;
	bool justLanded;
	bool justDamaged;
	bool justFallen;

	bool edge;

	bool stateJustChanged;	// ���°� �� �ٲ������ ����. update������ �ݺ����� �ʰ� �ϱ� ����
	bool isSummoned;		// ������ pool�� ���� Summon�Ǿ����� ����

	void changeState(Shooter2State changedState);

	void Buckshot(int bullets);
	double Radian(double degree);


private:
	void Initialize(D2DAnimatedSprite* animatedSprite);

public:
	void SetDirection();

	Shooter2State state;	// ������ ����. idle, walking ��

	static GameObject* CreateShooter2(Vector3d pos);	// ������ �־�� �ϴ� �Լ�

	virtual void Damage(double damage) override;

	virtual void OnDeath() override;

	virtual void Start() override;

	virtual void Update() override;

	virtual void OnCollisionEnter2D(const Collision2D& collision) override;

	virtual void OnCollisionExit2D(const Collision2D& collision) override;
};

