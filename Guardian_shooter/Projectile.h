#pragma once
#include "YunutyEngine.h"
#include "Timer.h"

enum class ProjectileType
{
	BULLET,
	BOMB,
	MISSILE,
	BOSSBULLET,
};

class Projectile : public Component
{
public:
	Projectile();
	virtual ~Projectile();
	static void RemoveAllProjectiles();
	Vector3d speed;
	double damage;
	bool isPlayerProj;
	D2DAnimatedSprite* animatedSprite;
	ProjectileType projType;

public:
	virtual void Initalize(D2DAnimatedSprite* sprite) = 0;
	virtual void Destroy() = 0;
	bool IsTimerOn() { return isTimerOn; }

protected:
	Timer destroyAnimTimer;
	bool isTimerOn = false;

private:
	static unordered_set<Projectile*> projectiles;
};