#pragma once
#include "YunutyEngine.h"
#include "Shooter3.h"

class ShootingLaser : public Component
{
private:
	void Initialize(D2DAnimatedSprite* animatedSprite);
	double damage = 1;
public:
	static GameObject* CreateLaser(Vector3d pos);

protected:
	D2DAnimatedSprite* animatedSprite = nullptr;
	virtual void Start() override;
	virtual void Update() override;
	virtual void OnCollisionEnter2D(const Collision2D& collision) override;
	virtual void OnCollisionStay2D(const Collision2D& collision) override;

};

