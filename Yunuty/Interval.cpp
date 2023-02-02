#pragma once
#include "Interval.h"
#include "YunutyEngine.h"

using namespace YunutyEngine;
using namespace YunutyEngine::YunutyMath;
Interval::Interval(double left, double right) :
    left(left), right(right) {}
Interval Interval::FromRadius(double centre, double radius)
{
    return Interval(centre - radius, centre + radius);
}
bool Interval::OverlappedWith(const Interval& other)const
{
    return other.right > right && other.left < right ||
        other.left < left&& other.right > left ||
        other.left > left && other.right < right || // other�� ����
        other.left<left&& other.right > right;// other���� ���Ե�
}
double Interval::GetLength()const
{
    return right - left;
}
