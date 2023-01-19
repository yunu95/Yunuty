#pragma once
#include "Interval.h"
#include "YunutyEngine.h"

using namespace YunutyEngine;
using namespace YunutyEngine::YunutyMath;
Interval::Interval(double centre, double radius) :
    left(centre - radius), right(centre + radius) {}
bool Interval::OverlappedWith(const Interval& other)
{
    return other.right > right && other.left < right ||
        other.left < left&& other.right > left ||
        other.left > left && other.right < right || // other�� ����
        other.left<left&& other.right > right;// other���� ���Ե�
}
