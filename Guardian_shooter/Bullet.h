#pragma once
#include "YunutyEngine.h"

class Bullet : public Component
{
public:
	Bullet() : speed(Vector3d(-200, 0, 0)), damage(3.f) { }
	Vector3d speed;
	double damage;
	bool isPlayerBullet;

protected:
	virtual void Update() override;
	virtual void OnCollisionEnter2D(const Collision2D& collision) override;
	virtual void OnCollisionExit2D(const Collision2D& collision) override;
};

void CreateBullet(Scene*, Vector3d);
