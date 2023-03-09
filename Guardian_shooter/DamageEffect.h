#pragma once
#include "YunutyEngine.h"
#include "Timer.h"

class DamageEffect : public Component
{
private:
	D2DAnimatedSprite* anim;
	void Initialize(D2DAnimatedSprite* animatedSprite);
public:
	static GameObject* CreateDamageEffect(Vector3d pos);

protected:
	virtual void Start() override;
	virtual void Update() override;

};

