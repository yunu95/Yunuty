#pragma once

#ifdef YUNUTY_EXPORTS
#define YUNUTY_API __declspec(dllexport)
#else
#define YUNUTY_API __declspec(dllimport)
#endif

using namespace YunutyEngine;

namespace YunutyEngine
{
    template<typename real>
    class Vector2;
    template<typename real>
    class Vector3
    {
    private:
    public:
        real x;
        real y;
        real z;
        Vector3();
        Vector3(real x, real y, real z);
        Vector3(const Vector3& rhs);
        Vector3(const Vector2<real>& rhs);
        static const Vector3 zero;
        static const Vector3 one;
        static const Vector3 right;
        static const Vector3 left;
        static const Vector3 up;
        static const Vector3 down;
        static const Vector3 forward;
        static const Vector3 backward;
        static real Dot(const Vector3& lhs, const Vector3& rhs);
        static Vector3 Project(const Vector3& vector, const Vector3& onNormal);
        static Vector3 Lerp(const Vector3& a, const Vector3& b, float t);
        static Vector3 Normalize(Vector3 v);
        double MagnitudeSqr()const;
        double Magnitude()const;
        Vector3 Normalized() const;

        Vector3& operator=(const Vector2<real>& rhs);
        Vector3& operator=(const Vector3& rhs);
        bool operator==(const Vector3& rhs) const;
        bool operator!=(const Vector3& rhs)const;
        Vector3& operator+=(const Vector2<real>& rhs);
        Vector3 operator+(const Vector2<real>& rhs) const;
        Vector3& operator+=(const Vector3& rhs);
        Vector3 operator+(const Vector3& rhs) const;
        Vector3& operator-=(const Vector2<real>& rhs);
        Vector3 operator-(const Vector2<real>& rhs) const;
        Vector3& operator-=(const Vector3& rhs);
        Vector3 operator-(const Vector3& rhs) const;
        Vector3 operator-()const;
        Vector3& operator*=(const double& scalar);
        Vector3& operator/=(const double& scalar);
        Vector3 operator*(const double& scalar)const;
        Vector3 operator/(const double& scalar)const;
    };
    typedef Vector3<float> Vector3f;
    typedef Vector3<double> Vector3d;
    typedef Vector3<int> Vector3i;

    template<typename real>
    YunutyEngine::Vector3<real> operator*(const double& scalar, const Vector3<real>& vector)
    {
        Vector3<real> ret(vector);
        ret *= scalar;
        return ret;
    }
};
template<typename real>
Vector3<real>::Vector3<real>() :Vector3<real>(0, 0, 0) {}
template<typename real>
Vector3<real>::Vector3<real>(real x, real y, real z)
{
    this->x = x;
    this->y = y;
    this->z = z;
}
template<typename real>
Vector3<real>::Vector3<real>(const Vector2<real>& rhs) : Vector3<real>(rhs.x, rhs.y, 0)
{ }
template<typename real>
Vector3<real>::Vector3<real>(const Vector3<real>& rhs) : Vector3<real>(rhs.x, rhs.y, rhs.z)
{ }
template<typename real>
real Vector3<real>::Dot(const Vector3<real>& lhs, const Vector3<real>& rhs)
{
    return lhs.x * rhs.x + lhs.y * rhs.y + lhs.z * rhs.z;
}
template<typename real>
Vector3<real> Vector3<real>::Project(const Vector3<real>& vector, const Vector3<real>& onNormal)
{
    double num = onNormal.MagnitudeSqr();
    if (num == 0)
        return Vector3::zero;

    double num2 = Vector3<real>::Dot(vector, onNormal);
    return Vector3<real>(onNormal.x * num2 / num, onNormal.y * num2 / num, onNormal.z * num2 / num);
}
template<typename real>
Vector3<real> Vector3<real>::Lerp(const Vector3<real>& a, const Vector3<real>& b, float t)
{
    return (1 - t) * a + t * b;
}
template<typename real>
Vector3<real> Vector3<real>::Normalize(Vector3<real> v)
{
    double size = v.Magnitude();
    return size != 0 ? v / size : Vector3<real>::zero;
}

template<typename real>
double Vector3<real>::MagnitudeSqr()const
{
    return x * x + y * y + z * z;
}
template<typename real>
double Vector3<real>::Magnitude()const
{
    return sqrt(MagnitudeSqr());
}
template<typename real>
Vector3<real> Vector3<real>::Normalized() const
{
    return Normalize(*this);
}

template<typename real>
Vector3<real>& Vector3<real>::operator=(const Vector2<real>& rhs)
{
    x = rhs.x;
    y = rhs.y;
    z = 0;
    return *this;
}
template<typename real>
Vector3<real>& Vector3<real>::operator=(const Vector3<real>& rhs)
{
    x = rhs.x;
    y = rhs.y;
    z = rhs.z;
    return *this;
}
template<typename real>
bool Vector3<real>::operator==(const Vector3<real>& rhs) const
{
    return x == rhs.x && y == rhs.y && z == rhs.z;
}
template<typename real>
bool Vector3<real>::operator!=(const Vector3<real>& rhs)const
{
    return !((*this) == rhs);
}
template<typename real>
Vector3<real>& Vector3<real>::operator+=(const Vector2<real>& rhs)
{
    this->x += rhs.x;
    this->y += rhs.y;
    return *this;
}
template<typename real>
Vector3<real> Vector3<real>::operator+(const Vector2<real>& rhs) const
{
    Vector3<real> ret(*this);
    ret += rhs;
    return ret;
}
template<typename real>
Vector3<real>& Vector3<real>::operator+=(const Vector3<real>& rhs)
{
    this->x += rhs.x;
    this->y += rhs.y;
    this->z += rhs.z;
    return *this;
}
template<typename real>
Vector3<real> Vector3<real>::operator+(const Vector3<real>& rhs) const
{
    Vector3<real> ret(*this);
    ret += rhs;
    return ret;
}
template<typename real>
Vector3<real>& Vector3<real>::operator-=(const Vector2<real>& rhs)
{
    this->x -= rhs.x;
    this->y -= rhs.y;
    return *this;
}
template<typename real>
Vector3<real> Vector3<real>::operator-(const Vector2<real>& rhs) const
{
    Vector3<real> ret(*this);
    ret -= rhs;
    return ret;
}
template<typename real>
Vector3<real>& Vector3<real>::operator-=(const Vector3<real>& rhs)
{
    this->x -= rhs.x;
    this->y -= rhs.y;
    this->z -= rhs.z;
    return *this;
}
template<typename real>
Vector3<real> Vector3<real>::operator-(const Vector3<real>& rhs) const
{
    Vector3<real> ret(*this);
    ret -= rhs;
    return ret;
}
template<typename real>
Vector3<real> Vector3<real>::operator-()const
{
    return *this * -1;
}
template<typename real>
Vector3<real>& Vector3<real>::operator*=(const double& scalar)
{
    this->x *= scalar;
    this->y *= scalar;
    this->z *= scalar;
    return *this;
}
template<typename real>
Vector3<real>& Vector3<real>::operator/=(const double& scalar)
{
    this->x /= scalar;
    this->y /= scalar;
    this->z /= scalar;
    return *this;
}
template<typename real>
Vector3<real> Vector3<real>::operator*(const double& scalar)const
{
    Vector3<real> ret(*this);
    ret *= scalar;
    return ret;
}
template<typename real>
Vector3<real> Vector3<real>::operator/(const double& scalar)const
{
    Vector3<real> ret(*this);
    ret /= scalar;
    return ret;
}

template<typename real>
const Vector3<real> Vector3<real>::one = Vector3<real>(1, 1, 0);
template<typename real>
const Vector3<real> Vector3<real>::zero = Vector3<real>(0, 0, 0);
template<typename real>
const Vector3<real> Vector3<real>::right = Vector3<real>(1, 0, 0);
template<typename real>
const Vector3<real> Vector3<real>::left = Vector3<real>(-1, 0, 0);
template<typename real>
const Vector3<real> Vector3<real>::up = Vector3<real>(0, 1, 0);
template<typename real>
const Vector3<real> Vector3<real>::down = Vector3<real>(0, -1, 0);
template<typename real>
const Vector3<real> Vector3<real>::forward = Vector3<real>(0, 0, 1);
template<typename real>
const Vector3<real> Vector3<real>::backward = Vector3<real>(0, 0, -1);
