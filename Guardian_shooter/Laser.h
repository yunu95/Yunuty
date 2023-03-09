#pragma once
#include "YunutyEngine.h"
#include "Timer.h"

class Laser : public Component
{
private:
	double damage = 1;
public:
	void Initialize(D2DAnimatedSprite* upperAnimatedSprite);
	static GameObject* CreateLaser(Vector3d pos);
	Timer attackTimer;			// 레이저가 유지되는 타이머

protected:
	D2DAnimatedSprite* animatedSprite = nullptr;
	D2DAnimatedSprite* lowerAnimatedSprite = nullptr;
	virtual void Start() override;
	virtual void Update() override;
	virtual void OnCollisionEnter2D(const Collision2D& collision) override;
	virtual void OnCollisionStay2D(const Collision2D& collision) override;

};

