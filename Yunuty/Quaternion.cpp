#include "YunutyEngine.h"
#include "Quaternion.h"
#include <math.h>
#include <cmath>
using namespace YunutyEngine;

Quaternion::Quaternion() :Quaternion(1, 0, 0, 0)
{ }
Quaternion::Quaternion(double w, double x, double y, double z)
{
    this->w = w;
    this->x = x;
    this->y = y;
    this->z = z;
}
Quaternion::Quaternion(const Vector3d& euler)
{
    *this = euler;
}
Quaternion& Quaternion::operator=(const Vector3d& euler)
{
    ;
    double cr = cos(euler.x * YunutyMath::Deg2Rad * 0.5);
    double sr = sin(euler.x * YunutyMath::Deg2Rad * 0.5);
    double cp = cos(euler.y * YunutyMath::Deg2Rad * 0.5);
    double sp = sin(euler.y * YunutyMath::Deg2Rad * 0.5);
    double cy = cos(euler.z * YunutyMath::Deg2Rad * 0.5);
    double sy = sin(euler.z * YunutyMath::Deg2Rad * 0.5);
    w = cr * cp * cy + sr * sp * sy;
    x = sr * cp * cy - cr * sp * sy;
    y = cr * sp * cy + sr * cp * sy;
    z = cr * cp * sy - sr * sp * cy;

    return *this;
}
Vector3d Quaternion::Euler() const
{
    Vector3d angles;

    // x (x-axis rotation)
    double sinr_cosp = 2 * (w * x + y * z);
    double cosr_cosp = 1 - 2 * (x * x + y * y);
    angles.x = std::atan2(sinr_cosp, cosr_cosp);

    // y (y-axis rotation)
    double sinp = 2 * (w * y - z * x);
    if (std::abs(sinp) >= 1)
        angles.y = std::copysign(YunutyMath::PI / 2, sinp); // use 90 degrees if out of range
    else
        angles.y = std::asin(sinp);

    // z (z-axis rotation)
    double siny_cosp = 2 * (w * z + x * y);
    double cosy_cosp = 1 - 2 * (y * y + z * z);
    angles.z = std::atan2(siny_cosp, cosy_cosp);

    return angles * YunutyMath::Rad2Deg;
}
Quaternion Quaternion::operator*(const Quaternion& other)const
{
    Quaternion ret;
    ret.w = w * other.w - x * other.x - y * other.y - z * other.z;
    ret.x = y * other.z - z * other.y + w * other.x + x * other.w;
    ret.y = z * other.x - x * other.z + w * other.y + y * other.w;
    ret.z = x * other.y - y * other.x + w * other.z + z * other.w;
    return ret;
}
Quaternion Quaternion::inverse()const
{
    return Quaternion(w, -x, -y, -z);
}
Vector3d Quaternion::operator*(const Vector3d& other)const
{
    auto ret = (*this) * Quaternion(0, other.x, other.y, other.z)*this->inverse();
    return Vector3d(ret.x, ret.y, ret.z);
}
Vector3d Quaternion::Forward()const
{
    return *this * Vector3d::forward;
}
Vector3d Quaternion::Up()const
{
    return *this * Vector3d::up;
}
Vector3d Quaternion::Right()const
{
    return *this * Vector3d::right;
}
