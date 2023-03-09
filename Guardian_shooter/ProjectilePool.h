#pragma once
#include "YunutyEngine.h"
#include "Projectile.h"
#include "map"


class ProjectilePool : public Component
{
public:
	ProjectilePool() { instance = this; }
	static ProjectilePool* GetInstance() { return instance; }
	Projectile* SummonProjectile(Vector3d position, Vector3d speed, ProjectileType projType, double damage, bool isPlayerBullet);
	void Retrieve(Projectile* projectile);
	void RetriveAll();

private:
	static ProjectilePool* instance;
	std::multimap<ProjectileType, Projectile*> projectiles;

};