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

	Enemy* SummonEnemy(Vector3d position, Vector3d speed);		// ���� ��ȯ ��ġ �� �ӵ� �Ű�����

	void Retrieve(Enemy* enemy);	
};

