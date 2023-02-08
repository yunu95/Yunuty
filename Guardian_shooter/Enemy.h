#pragma once
#include "YunutyEngine.h"
#include "Timer.h"

enum class EnemyState		// 적군의 상태
{
	IDLE,
	JUMPING,
	WALKING,
	ATTACK
};

class Enemy : public Component
{
private:
	double gravity = 100;
	double damage = 10.0;
	double hp = 200.0;
	EnemyState state = EnemyState::JUMPING;
	const double horizontalAccel = 1000;

	// coolTime

	// 공격 -> IDLE로 전환하기 위한 타이머
	Timer attackTimer;
	Timer traceTimer;

public:
	static Enemy* instance;
	Vector3d speed;
	Vector3d playerVec;
	Enemy();
	~Enemy() {};
	void Damage(double damage);
	static Vector3d GetEnemyPos();
	void EnemyMove(Vector3d playerVec);
	void Initialize(D2DAnimatedSprite* upperAnimatedSprite, D2DAnimatedSprite* lowerAnimatedSprite);
protected:
	D2DAnimatedSprite* upperAnimatedSprite = nullptr;
	D2DAnimatedSprite* lowerAnimatedSprite = nullptr;

	virtual void Update() override;

	virtual void OnCollisionEnter2D(const Collision2D& collision) override;

	virtual void OnCollisionExit2D(const Collision2D& collision) override;

};

GameObject* CreateEnemy(Scene& scene, Vector3d pos);


