#pragma once
#include "YunutyEngine.h"
#include "Destructible.h"
#include "EUnitState.h"
#include "ERank.h"
#include "DamageInfo.h"
#include "Timer.h"

class RA2UnitGraphic;
class Unit : public Destructible
{
public:
    Unit();
    void AttackCommand(Destructible* target);
    void AttackMoveCommand(Vector2d coord);
    void MoveCommand(Vector2d coord);
    void DeployCommand();

    void ChangeUnitType();
    static void Spawn();
    void ReceiveDamage(DamageInfo damageInfo);

    int ownerIndex = 0;
    double attackDamage = 1;
    double attackCooltime = 1;
    double movementSpeed = 100;
    UnitState state = UnitState::Idle;
    string weaponId = "";
    double exp = 0;
    const vector<double>* expTable;
    Rank rank = Rank::Recruit;

    // �̰� �̾��ִ°� �ܺ��� ����
    NavigationUnit2D* navUnitComponent = nullptr;
    RA2UnitGraphic* unitGraphic = nullptr;

protected:
    void Update() override;
private:
    Collider2D* AttackRangeCollider = nullptr;

    Timer attackCooldownTimer;
    Timer movementTimer;
    queue<Vector2d> movePathQueue;
    void Attack(Destructible* target);
    void AttackMove(Vector2d coord);
    void Move(Vector2d coord);
    void Deploy();
};

