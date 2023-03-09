#pragma once
#include "Projectile.h"

class Bomb : public Projectile
{
public:
	Bomb();

public:
	static void CreateBomb(Vector3d pos);
	virtual void Initalize(D2DAnimatedSprite* sprite) override;
	virtual void Destroy() override;

protected:
	virtual void Update() override;
	virtual void OnCollisionEnter2D(const Collision2D& collision) override;
	virtual void OnCollisionExit2D(const Collision2D& collision) override;

private:
	const double gravity = 98.0;
	Timer explosionDamageTimer;
	bool isExplosionTimerEnd;
};
