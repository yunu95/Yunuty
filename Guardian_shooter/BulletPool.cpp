#include "BulletPool.h"
#include "Bullet.h"

BulletPool* BulletPool::instance = nullptr;

Bullet* BulletPool::SummonBullet(Vector3d position, Vector3d speed, double damage, bool isPlayerBullet)
{
	if (bullets.empty())
	{
		//return nullptr;
		CreateBullet(GetGameObject()->GetScene(), speed);
	}
	auto ret = bullets[0];
	ret->isPlayerBullet = isPlayerBullet;
	ret->damage = damage;
	ret->GetTransform()->SetWorldPosition(position);
	ret->speed = speed;
	ret->GetGameObject()->SetSelfActive(true);
	bullets.erase(bullets.begin());
	return ret;
}

void BulletPool::Retrieve(Bullet* bullet)
{
	bullets.push_back(bullet);
	bullet->GetGameObject()->SetSelfActive(false);
}