#pragma once
#include "YunutyEngine.h"

enum class PlayerMovementState
{
	IDLE = 0,
	WALK = 1,
	JUMP = 2,
	DASH = 3,
};

enum class PlayerAttackState
{
	NONE = 4,
	SHOOT = 5,
	MELEE = 6,
};


class Player : public Component
{
public:
	Player();
	~Player() {};
	BoxCollider2D* playerHitbox;
	BoxCollider2D* playerMeleeRange;

public:
	static Player* instance;
	static Vector3d GetPlayerPos();
	static void SetPlayerPos(Vector3d);
	void Initialize(D2DAnimatedSprite* upperAnimatedSprite, D2DAnimatedSprite* lowerAnimatedSprite);
	void SetCombo(size_t combo);
	void Damage(double damage);

protected:
	D2DAnimatedSprite* upperAnimatedSprite = nullptr;
	D2DAnimatedSprite* lowerAnimatedSprite = nullptr;
	//게임의 매 프레임마다 자동적으로 호출되는 콜백 함수
	virtual void Update() override;
	virtual void OnCollisionEnter2D(const Collision2D& collision) override;
	virtual void OnCollisionExit2D(const Collision2D& collision) override;

private:
	
	PlayerMovementState state = PlayerMovementState::JUMP;
	PlayerAttackState attackState = PlayerAttackState::NONE;
	Vector3d speed;
	double hp = 200;
	double gravity = 98.0;
	double damage = 10.0;

	//attack coolTime
	double elapsed = 0;
	// attack state
	bool isEnemyClosed = 0;
	// combo
	size_t combo = 0;
	// gauge
	size_t gauge = 0;
};

void CreatePlayer(Scene*, Vector3d);