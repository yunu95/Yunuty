#pragma once
#include "Threat.h"

class DamageEffect;

enum class EliteState
{
	IDLE,
	FALLING,
	WALKING,
	ATTACK,
	ATTACK2,
	DEATH
};

class EliteMonster : public Threat
{
private:
	D2DAnimatedSprite* anim;
	Vector3d speed;			// enemy�� �̵��ӵ�
	EliteState state;
	DamageEffect* effect = nullptr;

	Timer traceTimer;
	Timer attackDelay;
	Timer attackDelay2;
	Timer interrupted;
	Timer attackCoolDownTimer;
	Timer animTimer;		// ��� ���
	Timer changeIdle;
	Timer damageEffect;

	double damage;
	double gravity;
	int bulletCnt;
	char horizontalView;
	int attackPattern;

	int randHorRange;
	int randVerRange;
	bool canAttack;			// ���� ���� ����
	bool attackVerable;		// ���� ���� ���� ����
	bool attackHorable;		// ���� ���� ���� ����

	bool attackFinished;
	bool isBombShooted;
	bool isAttackReady;

	bool justLanded;
	bool justDamaged;
	bool justFallen;

	bool stateJustChanged;	// ���°� �� �ٲ������ ����. update������ �ݺ����� �ʰ� �ϱ� ����
	bool isSummoned;		// ������ pool�� ���� Summon�Ǿ����� ����

	void changeState(EliteState changedState);
	void Initialize(D2DAnimatedSprite* animatedSprite);
	int decidePattern(int& pattern);
protected:
	virtual void Damage(double damage) override;

	virtual void OnDeath() override;

	virtual void Start() override;

	virtual void Update() override;

	virtual void OnCollisionEnter2D(const Collision2D& collision) override;

	virtual void OnCollisionExit2D(const Collision2D& collision) override;
public:
	static GameObject* CreateEliteMonster(Vector3d pos);

};

