#include "Unit.h"
#include "YunuAlert2.h"

unordered_map<Collider2D*, Unit*> Unit::unitColliderMap;

Unit::Unit()
{
}
void Unit::AttackCommand(Destructible* target)
{
}
void Unit::AttackMoveCommand(Vector2d coord)
{
}
void Unit::MoveCommand(Vector2d coord)
{
    const Vector2d& source = GetTransform()->GetWorldPosition();
#ifdef _DEBUG
    unordered_set<const NavigationField2D::Tile*> openSet;
    unordered_set<const NavigationField2D::Tile*> closedSet;
    unordered_set<const NavigationField2D::Tile*> path;
    auto navField = NavigationField2D::GetAssignedField(source);
    movePathQueue = navField->RequestPath(source, coord, 30, &openSet, &closedSet, &path);
#else
    movePathQueue = NavigationField2D::GetAssignedField(source)->RequestPath(source, coord);
#endif
    if (state == UnitState::Dying)
        return;
    if (movePathQueue.empty())
    {
        unitGraphic->PlayIdle();
        return;
    }
#ifdef _DEBUG
    if (DebugTilePool::instance->poolObjectsSize() == DebugTilePool::instance->expendableObjectsSize())
    {
        double max_g = 0.0000001;
        for (auto each : path)
        {
            closedSet.erase(each);
            if (each->g_score > max_g)
                max_g = each->g_score;
        }
        for (auto each : closedSet)
        {
            auto borrowed = DebugTilePool::instance->Borrow(navField->TileToWorldLocaction(*each), navField->GetTileSize(), D2D1::ColorF::Orange);
            borrowed->SetGlowingOffset(0.5 - 0.5 * (each->g_score / max_g));
        }
        for (auto each : path)
        {
            auto borrowed = DebugTilePool::instance->Borrow(navField->TileToWorldLocaction(*each), navField->GetTileSize(), D2D1::ColorF::Green);
            borrowed->SetGlowingOffset(0.5 - 0.5 * (each->g_score / max_g));
        }
        for (auto each : openSet)
        {
            auto borrowed = DebugTilePool::instance->Borrow(navField->TileToWorldLocaction(*each), navField->GetTileSize(), D2D1::ColorF::Red);
            borrowed->SetGlowingOffset(0.5 - 0.5 * (each->g_score / max_g));
        }
    }
#else
#endif

    movementTimer.onFinished = [this, coord]() { this->navUnitComponent->ReportStatus(); this->MoveCommand(coord); };
    Move(movePathQueue.front());
    movePathQueue.pop();
}
void Unit::DeployCommand()
{
}

void Unit::ChangeUnitType()
{
}
void Unit::ReceiveDamage(DamageInfo damageInfo)
{
    hp -= damageInfo.damage;
    if (hp < 0)
        Kill();
}
void Unit::Kill()
{
    attackCooldownTimer.isActive = false;
    movementTimer.isActive = false;
    state = UnitState::Dying;
    unitGraphic->PlayDie();
}
void Unit::SetUnitCollider(Collider2D* unitCollider)
{
    this->unitCollider = unitCollider;
    unitColliderMap[unitCollider] = this;
}
Unit* Unit::GetUnitFromCollider(Collider2D* collider)
{
    if (unitColliderMap.find(collider) == unitColliderMap.end())
        return nullptr;
    return unitColliderMap[collider];
}

void Unit::Update()
{
    attackCooldownTimer.Update(Time::GetDeltaTime());
    movementTimer.Update(Time::GetDeltaTime());
}
void Unit::OnCollisionEnter2D(const Collision2D& collision)
{
    if (state == UnitState::Dying)
        return;

    auto otherUnit = GetUnitFromCollider(collision.m_OtherCollider);
    if (otherUnit == nullptr)
        return;
    if (otherUnit->state == UnitState::Dying)
        return;
    if (otherUnit->ownerIndex == this->ownerIndex)
        return;

    Attack(otherUnit);
}
void Unit::OnCollisionExit2D(const Collision2D& collision)
{
    if (state == UnitState::Dying)
        return;

    if (attackTarget == GetUnitFromCollider(collision.m_OtherCollider))
        StopAttack();
}

void Unit::Attack(Destructible* target)
{
    attackCooldownTimer.duration = attackCooltime;
    attackCooldownTimer.isRepeating = true;
    attackTarget = target;
    unitGraphic->PlayAttack();
    attackCooldownTimer.onUpdate = [this](double t)
    {
        unitGraphic->SetFacingAngle(Vector2d((attackTarget->GetTransform()->GetWorldPosition() - GetTransform()->GetWorldPosition())).GetAngleDegree());
    };
    attackCooldownTimer.onFinished = [this]()
    {
        if (((Unit*)attackTarget)->state == UnitState::Dying)
            return;
        Attack(attackTarget);
    };
    attackTarget->ReceiveDamage(DamageInfo(attackDamage));
    attackCooldownTimer.Start();
}
void Unit::StopAttack()
{
    attackCooldownTimer.isActive = false;
}
void Unit::AttackMove(Vector2d coord)
{
}
void Unit::Move(Vector2d coord)
{
    if (state == UnitState::Dying)
        return;
    auto src = GetTransform()->GetWorldPosition();
    auto transform = GetTransform();
    unitGraphic->SetFacingAngle((coord - src).GetAngleDegree());
    unitGraphic->PlayMove();
    movementTimer.duration = (coord - src).Magnitude() / movementSpeed;
    movementTimer.onUpdateNormalized = [src, coord, transform](double t) {transform->SetWorldPosition(Vector3d::Lerp(src, coord, t)); };
    movementTimer.Start();
}
void Unit::Deploy()
{
}
