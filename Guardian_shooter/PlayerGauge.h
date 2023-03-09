#pragma once
#include "YunutyEngine.h"

class PlayerGauge : public Component
{
public:
	static PlayerGauge* CreateGaugeBox(Vector3d pos);

private:
	Vector3d pos;

};

