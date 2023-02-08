#pragma once
#include "YunutyEngine.h"
#include <vector>

class Bullet;

class BulletPool : public Component
{
private:
	vector<Bullet*> bullets;

public:
	static BulletPool* instance;
	BulletPool()
	{
		instance = this;
	}

	Bullet* SummonBullet(Vector3d position, Vector3d speed, double damage, bool isPlayerBullet);

	void Retrieve(Bullet* bullet);


};

