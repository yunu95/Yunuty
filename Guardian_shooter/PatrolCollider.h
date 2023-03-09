#pragma once
#include "Ground.h"
#include "Threat.h"

class PatrolCollider : public Threat
{
public:
	static GameObject* CreatePatrolbox();
protected:
	virtual void OnCollisionEnter2D(const Collision2D& collision) override;
};

