#pragma once
#include "Threat.h"

class Puddle : public Component
{
private:
	void Initialize(D2DAnimatedSprite* upperAnimatedSprite, D2DAnimatedSprite* lowerAnimatedSprite);
	double damage = 1;
public:
	static GameObject* CreatePuddle(Vector3d pos);
protected:
	D2DAnimatedSprite* upperAnimatedSprite = nullptr;
	D2DAnimatedSprite* lowerAnimatedSprite = nullptr;

	virtual void Start() override;
	virtual void Update() override;
	virtual void OnCollisionEnter2D(const Collision2D& collision) override;
	virtual void OnCollisionStay2D(const Collision2D& collision) override;


};