#include "YunutyEngine.h"
#include "LineCollider2D.h"
using namespace YunutyEngine;

bool LineCollider2D::isOverlappingWith(BoxCollider2D* other)
{
    return Collider2D::isOverlapping(other, this);
}
bool LineCollider2D::isOverlappingWith(CircleCollider2D* other)
{
    return Collider2D::isOverlapping(other, this);
}
bool LineCollider2D::isOverlappingWith(LineCollider2D* other)
{
    return Collider2D::isOverlapping(other, this);
}
