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
	Vector3d speed;			// enemy의 이동속도
	EliteState state;
	DamageEffect* effect = nullptr;

	Timer traceTimer;
	Timer attackDelay;
	Timer attackDelay2;
	Timer interrupted;
	Timer attackCoolDownTimer;
	Timer animTimer;		// 사망 모션
	Timer changeIdle;
	Timer damageEffect;

	double damage;
	double gravity;
	int bulletCnt;
	char horizontalView;
	int attackPattern;

	int randHorRange;
	int randVerRange;
	bool canAttack;			// 공격 가능 여부
	bool attackVerable;		// 세로 공격 가능 여부
	bool attackHorable;		// 가로 공격 가능 여부

	bool attackFinished;
	bool isBombShooted;
	bool isAttackReady;

	bool justLanded;
	bool justDamaged;
	bool justFallen;

	bool stateJustChanged;	// 상태가 막 바뀌었는지 여부. update문에서 반복되지 않게 하기 위함
	bool isSummoned;		// 유닛이 pool을 거쳐 Summon되었는지 여부

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

