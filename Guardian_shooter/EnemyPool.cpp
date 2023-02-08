#include "EnemyPool.h"
#include "Enemy.h"

EnemyPool* EnemyPool::pinstance = nullptr;

Enemy* EnemyPool::SummonEnemy(Vector3d position, Vector3d speed)
{
	if (enemies.empty())
	{
		return nullptr;
	}
	auto ret = enemies[0];
	ret->GetTransform()->SetWorldPosition(position);
	ret->speed = speed;
	ret->GetGameObject()->SetSelfActive(true);
	enemies.erase(enemies.begin());
	return ret;
}

void EnemyPool::Retrieve(Enemy* enemy)
{
	enemies.push_back(enemy);
	enemy->GetGameObject()->SetSelfActive(false);
}