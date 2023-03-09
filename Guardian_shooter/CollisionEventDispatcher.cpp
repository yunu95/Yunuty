#include "CollisionEventDispatcher.h"

void CollisionEventDispatcher::OnCollisionEnter2D(const Collision2D& collision)
{
	target->OnCollisionEnter2D(collision);
}

void CollisionEventDispatcher::OnCollisionStay2D(const Collision2D& collision)
{
	target->OnCollisionStay2D(collision);
}

void CollisionEventDispatcher::OnCollisionExit2D(const Collision2D& collision)
{
	target->OnCollisionExit2D(collision);
}

