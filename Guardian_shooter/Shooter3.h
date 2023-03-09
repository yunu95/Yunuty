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

class Shooter3 : public Threat		// ������ ���� ���� ����
{
private:

	ShootingLaser* laser = nullptr;
	DamageEffect* effect = nullptr;

	D2DAnimatedSprite* anim;
	Vector3d speed;			// enemy�� �̵��ӵ�

	Timer traceTimer;
	Timer LaserLastTimer;
	Timer attackDelay;		// ������ ������ ����
	Timer animTimer;		// ��� ���
	Timer laserShoot;		// ��ź Ÿ�̸�
	Timer chargeStartTimer;
	Timer attackStartTimer;
	Timer laserLastTimer;
	Timer damageEffect;

	double gravity;
	int bulletCnt;

	int attackStart;		// 0�̸� ��ź
	bool canAttack;			// ���� ���� ����

	bool justLanded;
	bool justDamaged;
	bool justFallen;

	bool stateJustChanged;	// ���°� �� �ٲ������ ����. update������ �ݺ����� �ʰ� �ϱ� ����
	bool isSummoned;		// ������ pool�� ���� Summon�Ǿ����� ����

	void changeState(Shooter3State changedState);

private:
	void Initialize(D2DAnimatedSprite* animatedSprite);

protected:
	double damage;

public:

	Shooter3State state;	// ������ ����. idle, walking ��

	static GameObject* CreateShooter3(Vector3d pos);	// ������ �־�� �ϴ� �Լ�

	virtual void Damage(double damage) override;

	virtual void OnDeath() override;

	virtual void Start() override;

	virtual void Update() override;

	virtual void OnCollisionEnter2D(const Collision2D& collision) override;

	virtual void OnCollisionExit2D(const Collision2D& collision) override;
};

