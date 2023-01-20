#include "Unit.h"
#include "YunuAlert2.h"

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
    if (movePathQueue.empty())
        return;
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
void Unit::Spawn()
{
}
void Unit::ReceiveDamage(DamageInfo damageInfo)
{
}

void Unit::Update()
{
    attackCooldownTimer.Update(Time::GetDeltaTime());
    movementTimer.Update(Time::GetDeltaTime());
}

void Unit::Attack(Destructible* target)
{
}
void Unit::AttackMove(Vector2d coord)
{
}
void Unit::Move(Vector2d coord)
{
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
