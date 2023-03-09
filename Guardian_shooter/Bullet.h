#pragma once
#include "YunutyEngine.h"
#include "Projectile.h"

class Bullet : public Projectile
{
public:
	Bullet();

public:
	static void CreateBullet(Vector3d);
	virtual void Initalize(D2DAnimatedSprite* sprite) override;
	virtual void Destroy() override;

protected:
	virtual void Update() override;
	virtual void OnCollisionEnter2D(const Collision2D& collision) override;
	virtual void OnCollisionExit2D(const Collision2D& collision) override;

private:
	
};


