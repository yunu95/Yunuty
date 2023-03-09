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
    // ���ظ� ������, hp�� ���, hp�� 0 ���ϸ� �״´�.
    virtual void Damage(double damage);
    // �������� �ް� hp�� 0 ���ϰ� �Ǹ� OnDeath�� ȣ��Ǵµ�,
    // ����� ������ ���� ������ ������ ���� �ٸ� �� �ִ�.
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
