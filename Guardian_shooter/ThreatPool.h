#pragma once
#include "YunutyEngine.h"
#include "Threat.h"
#include "map"

class ThreatPool : public Component
{
public:
	ThreatPool() { instance = this; }
	static ThreatPool* GetInstance() { return instance; }
	Threat* SummonThreat(Vector3d position, ThreatType threatType);
	void Retrieve(Threat* threat);
private:
	static ThreatPool* instance;
	std::multimap<ThreatType, Threat*> threats;
};