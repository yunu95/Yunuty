#pragma once
#include "YunutyEngine.h"
#include <vector>

class Enemy;

class EnemyPool : public Component
{
private:
	vector<Enemy*> enemies;

public:
	static EnemyPool* pinstance;
	EnemyPool()
	{
		pinstance = this;
	}

	Enemy* SummonEnemy(Vector3d position, Vector3d speed);		// 적군 소환 위치 및 속도 매개변수

	void Retrieve(Enemy* enemy);	
};

