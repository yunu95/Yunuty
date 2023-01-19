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
    movePathQueue = NavigationField2D::GetAssignedField(source)->RequestPath(source, coord);
    if (movePathQueue.empty())
        return;

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
