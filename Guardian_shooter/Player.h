#pragma once
#include "YunutyEngine.h"
#include "Timer.h"
#include "unordered_set"

namespace UI
{
	class UIManager;
}

class Ground;
class Threat;
class Projectile;
class Drone;
class Threat;

enum class PlayerMovementState
{
	IDLE,
	WALK,
	JUMP,
	DASH,
	DEATH
};

enum class PlayerAttackState
{
	NONE,
	SHOOT,
	MELEE,
	SKILL,
	SKILL2
};
namespace UI
{
	class UIManager;
}


class Player : public Component
{
	friend class UI::UIManager;
public:
	Player();
	~Player() {};
	BoxCollider2D* playerHitbox;
	PlayerMovementState GetMovementState() { return state; }
	BoxCollider2D* playerMeleeHitBox;	
	BoxCollider2D* playerSkillHitBox;

	// drone
	bool isDroneSummoned = false;

public:
	static Player* GetInstance() { return instance; };
	static Vector3d GetPlayerPos();
	static void SetPlayerPos(Vector3d);
	static void SetPlayerSpeed(Vector3d);
	static char GetPlayerDirection();
	Vector3d GetPlayerFootPos(Vector3d playerPos = Player::GetPlayerPos());
	void Reset();
	void ResetPlayerStatus();
	void Initialize(D2DAnimatedSprite* upperAnimatedSprite, D2DAnimatedSprite* lowerAnimatedSprite, D2DAnimatedSprite* damagedAnim);
	void Damage();
	void Hit();
	void Die();
	void Revive();
	void Revive(Vector3d pos);
	Vector3d GetRevivalPointPos();
	void AddCombo(size_t value);
	void SetGauge(size_t damage);
	void Heal(double value);
	void AttackSpeedUp();
	Vector3d GetPlayerSpeed();

	static GameObject* CreatePlayer(Vector3d pos);

protected:
	D2DAnimatedSprite* upperAnimatedSprite = nullptr;
	D2DAnimatedSprite* lowerAnimatedSprite = nullptr;
	D2DAnimatedSprite* damagedAnimatedSprite = nullptr;
	//게임의 매 프레임마다 자동적으로 호출되는 콜백 함수
	//virtual void Start() override;
	virtual void Update() override;
	virtual void OnCollisionEnter2D(const Collision2D& collision) override;
	virtual void OnCollisionStay2D(const Collision2D& collision) override;
	virtual void OnCollisionExit2D(const Collision2D& collision) override;
private:
	static Player* instance;
	//Threat* tinstance;
	GameObject* playerChildObject;
	GameObject* damagedAnimObj;
    double skillRushDirection = 1;
    D2DAnimatedSprite* skillRushTorsos[5] = {nullptr};
    D2DAnimatedSprite* skillRushLegs[5]= {nullptr};
    bool skillAfterImageAnimated[5];

	PlayerMovementState state = PlayerMovementState::JUMP;
	PlayerAttackState attackState = PlayerAttackState::NONE;
	Vector3d speed;
	Vector3d lastFramePlayerPos = Vector3d::zero;
	
	const double gravity = 98.0;

	char horizontalView = 1; //1 = right, -1 = left	
	char verticalView = 0;	 //0 = forward, 1 = up, -1 = down
	int hp = 3;
	const double maxHp = 3.0;
	int lifeCount = 2;
	const int maxLifeCount = 2;
	const size_t maxGauge = 120;
	const double jumpForce = 50.0;
	const double shootDamage = 1.0;
	const double meleeDamage = 2.0;
	const double skillDamage = 10.0;
	const double horizontalWalkSpeed = 300;
	const double dashSpeed = 1400;
    const double skillRushLength=100;
	double attackSpeed = 1.0;

	//coolTime
	Timer dashCoolDowmTimer;
	Timer attackCoolDownTimer;
	Timer meleeCoolDownTimer;
	Timer skillCoolDownTimer;
	Timer comboTimer;
    Timer skillAfterImageTimer;

	Timer invincibleTimerOnRevive;
	Timer invincibleTimer;
	Timer deathAnimTimer;

	Timer skillCameraShakeTimer;
	Timer parryCameraSlowTimer;
	Vector3d originCameraPos;

	Timer attackSpeedUpTimer;
	Timer droneLastTimer;
	Drone* pdrone = nullptr;

	bool isInvincible = false;
	bool isDying = false;

	bool isMoving = false;
	bool dashFinished = true;
	bool isDashReady = true;
	bool groundStepped = false;
	// attack state
	bool attackFinished = true;
	bool attackJustStarted = false;
	bool meleeFinished = true;
	bool skillFinished = true;

	

	bool isAttackSpeedUp = false;
	// 게임 로드시 예외처리를 위한 불 함수
	bool skipFrame = false;
	static bool autoShot;

	// combo
	size_t combo = 0;
	// gauge
	double gaugeFilledSpeed = 1.0;
	size_t gauge = 0;
	size_t skillCount = 0;

	std::unordered_set<Ground*> steppedGrounds;
	std::unordered_set<Threat*> closeEnemies;
	std::unordered_set<Threat*> closeEnemiesSetForSkill;
	std::unordered_set<Projectile*> projectilesToParry;
	Ground* steppedGroundAtLast;
	friend Ground;
};
