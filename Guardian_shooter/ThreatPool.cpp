#include "Guardian_shooter.h"
#include "ThreatPool.h"
#include "ShooterOneShot.h"
#include "Bomber.h"
#include "Shooter1.h"
#include "Shooter2.h"
#include "Shooter3.h"
#include "Boss.h"
#include "EliteMonster.h"


ThreatPool* ThreatPool::instance = nullptr;

Threat* ThreatPool::SummonThreat(Vector3d position, ThreatType threatType)
{
	auto iter = threats.find(threatType);
	if (iter == threats.end())
	{
		switch (threatType)
		{
			case ThreatType::SHOOTER_ONESHOT:
				ShooterOneShot::CreateShooterOneShot(position);
				break;
			case ThreatType::SHOOTER_TRIPLESHOT:
				Shooter1::CreateShooter1(position);
				break;
			case ThreatType::SHOOTER_BUCKSHOT:
				Shooter2::CreateShooter2(position);
				break;
			case ThreatType::SHOOTER_CURVE:
				Bomber::CreateBomber(position);
				break;
			case ThreatType::SHOOTER_LASER:
				Shooter3::CreateShooter3(position);
				break;
			case ThreatType::ELITE_MONSTER:
				EliteMonster::CreateEliteMonster(position);
				break;
			case ThreatType::BOSS:
				Boss::CreateBoss(position);
				break;
			case ThreatType::OBSTACLE:
				break;
			default:
				break;
		}
		iter = threats.find(threatType);
	}
	auto ret = iter->second;
	ret->GetTransform()->SetWorldPosition(position);
	ret->GetGameObject()->SetSelfActive(true);
	threats.erase(iter);
	return ret;
}

void ThreatPool::Retrieve(Threat* threat)
{
	threats.insert(std::make_pair(threat->threatType, threat));
	if (threat->belongingWave)
		threat->belongingWave->HandleDeathEvent(threat);

	threat->GetGameObject()->SetSelfActive(false);
}