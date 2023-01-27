#pragma once
#include <unordered_map>
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

    void SetUnitCollider(Collider2D* unitCollider);
    void ChangeUnitType();
    virtual void ReceiveDamage(DamageInfo damageInfo)override;
    virtual void Kill()override;

    static Unit* GetUnitFromCollider(Collider2D* collider);

    int ownerIndex = 0;
    double attackDamage = 1;
    double hp = 5;
    double attackCooltime = 1;
    double movementSpeed = 100;
    UnitState state = UnitState::Idle;
    string weaponId = "";
    double exp = 0;
    const vector<double>* expTable;
    Rank rank = Rank::Recruit;

    // 이걸 이어주는건 외부의 역할
    NavigationUnit2D* navUnitComponent = nullptr;
    RA2UnitGraphic* unitGraphic = nullptr;

protected:
    void Update() override;
    void OnCollisionEnter2D(const Collision2D& collision)override;
    void OnCollisionExit2D(const Collision2D& collision)override;
private:
    Collider2D* unitCollider = nullptr;
    Collider2D* attackRangeCollider = nullptr;
    Destructible* attackTarget = nullptr;

    Timer attackCooldownTimer;
    Timer movementTimer;
    queue<Vector2d> movePathQueue;
    static unordered_map<Collider2D*, Unit*> unitColliderMap;
    void Attack(Destructible* target);
    void StopAttack();
    void AttackMove(Vector2d coord);
    void Move(Vector2d coord);
    void Deploy();
};

