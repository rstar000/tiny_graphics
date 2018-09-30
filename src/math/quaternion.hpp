// Class that defines a quaternion and its relation to vector rotation

#ifndef QUATERNION_HPP
#define QUATERNION_HPP

#include <cmath>
#include "vector.hpp"
#include "matrix.hpp"

namespace tmath
{

class Quaternion
{
    Vec3 v;
    float w;
public:
    Quaternion() : v(Vec3()), w(1.0f) { }
    Quaternion(const Quaternion &other) : v(other.v), w(other.w) { }
    Quaternion(float x, float y, float z, float w) : v(Vec3({x,y,z})), w(w) { }
    Quaternion(const Vec3 &v, float w) : v(v), w(w) { }

    Vec3& getV() { return v; }
    Vec3 getV() const { return v; }
    float& getW() { return w; }
    float getW() const { return w; }

    Quaternion operator+ (const Quaternion &other) const
    {
        return Quaternion(v + other.v, w + other.w);
    }

    Quaternion operator- (const Quaternion &other) const
    {
        return Quaternion(v - other.v, w - other.w);
    }

    Quaternion operator* (const Quaternion &other) const
    {
        float sc = w * other.w - dot(v, other.v);
        Vec3 vec = w * other.v + other.w * v + cross(v, other.v);
        return Quaternion(vec, sc);
    }
};

using Quat = Quaternion;

inline Quaternion operator* (float x, const Quaternion &q)
{
    return Quaternion(x * q.getV(), x * q.getW());
}

inline Quaternion conj(const Quaternion &q)
{
    return Quaternion(-q.getV(), q.getW());
}

inline float norm(const Quaternion &q)
{
    const Vec3 &v = q.getV();
    float w = q.getW();
    return sqrt(dot(v,v) + w*w);
}

inline Quaternion inverse(const Quaternion &q)
{
    float n = length2(q.getV());
    return (1.0f / n) * conj(q);
}


inline Vec3 rotate(const Vec3 vec, const Quaternion &q)
{
    Vec3 v = q.getV();
    float w = q.getW();
    float a = w*w - length2(v);
    float b = 2.0f * (dot(v, vec));
    float c = 2.0f * w;

    return  (a*vec + b*v + c*cross(v, vec));
}

inline Quaternion angle_axis(float angle, const Vec3 &axis)
{
    float w = cos(angle / 2.0f);
    float s = sin(angle / 2.0f);

    Vec3 v = s * normalize(axis);
    return Quaternion(v,w);
}

inline Mat4 quat2matrix(const Quat &q)
{
    Vec4 v1 = toVec4(rotate(Vec3({1,0,0}), q), 0.0f);
    Vec4 v2 = toVec4(rotate(Vec3({0,1,0}), q), 0.0f);
    Vec4 v3 = toVec4(rotate(Vec3({0,0,1}), q), 0.0f);
    Vec4 v4({0,0,0,1});

    return Mat4({v1,v2,v3,v4});
}

inline Quat look_at(const Vec3 &from, const Vec3 &to)
{
    Vec3 z({0,0,1});
    Vec3 dir = normalize(to - from);
    float angle = acos(dot(dir, z));
    Vec3 axis = cross(z,dir);
    return angle_axis(angle, axis);
}

}
#endif
