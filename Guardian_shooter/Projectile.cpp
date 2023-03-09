#include "Projectile.h"

unordered_set<Projectile*> Projectile::projectiles;

Projectile::Projectile()
{
	projectiles.insert(this);
}

Projectile::~Projectile()
{
	projectiles.erase(this);
}

void Projectile::RemoveAllProjectiles()
{
	for (auto each : projectiles)
	{
		Scene::getCurrentScene()->DestroyGameObject(each->GetGameObject());
	}
}

