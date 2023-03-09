#pragma once
#include "YunutyEngine.h"
#include "Timer.h"

enum class ThreatType
{
    SHOOTER_ONESHOT,
    SHOOTER_TRIPLESHOT,
    SHOOTER_BUCKSHOT,
    SHOOTER_CURVE,
    SHOOTER_LASER,
    ELITE_MONSTER,
    BOSS,
    OBSTACLE,
};
class ThreatPool;
namespace gs_map
{
    class EnemyMarker;
    class EnemyWave;
}

class Threat : public Component
{
protected:
    bool invincible = false;

public:
    static void RemoveAllThreats();
    Threat();
	double hp = 2;

    virtual ~Threat();
    // 피해를 입으면, hp를 깎고, hp가 0 이하면 죽는다.
    virtual void Damage(double damage);
    // 데미지를 받고 hp가 0 이하가 되면 OnDeath가 호출되는데,
    // 사망시 동작할 논리는 위협의 종류에 따라 다를 수 있다.
    virtual void OnDeath() = 0;
    double GetHp() { return hp; }
    ThreatType threatType;
    bool isDroneSummoned;
private:
    static unordered_set<Threat*> threats;
    gs_map::EnemyMarker* birthMother = nullptr;
    gs_map::EnemyWave* belongingWave = nullptr;
    friend gs_map::EnemyMarker;
    friend gs_map::EnemyWave;
    friend ThreatPool;
};
