#pragma once
#include "YunutyEngine.h"
#include "Projectile.h"

class BossBullet : public Projectile
{
public:
	BossBullet();

public:
	static void CreateBossBullet(Vector3d);
	virtual void Initalize(D2DAnimatedSprite* sprite) override;
	virtual void Destroy() override;

protected:
	virtual void Update() override;
	virtual void OnCollisionEnter2D(const Collision2D& collision) override;
	virtual void OnCollisionExit2D(const Collision2D& collision) override;

private:

};


