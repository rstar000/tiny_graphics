#include "matrix.hpp"
#include "quaternion.hpp"

#ifndef TRANSFORM_HPP
#define TRANSFORM_HPP

namespace tmath
{

inline Matrix<4,4> eye()
{
    Mat4 res;
    for(int i = 0; i < 4; i++) { res(i,i) = 1.0f; }
    return res;
}

inline Mat4 translation(const Vec3 &delta)
{
    Mat4 res = eye();
    for(int i = 0; i < 3; i++) {
        res(i, 3) = delta[i];
    }
    return res;
}

inline Mat4 rotation(float angle, const Vec3 &axis)
{
    Quat q = angle_axis(angle, axis);
    return quat2matrix(q);
}

inline Mat4 scaling(float sx, float sy, float sz) {
    Mat4 res = eye();
    res(0,0) = sx;
    res(1,1) = sy;
    res(2,2) = sz;
    return res;
}

inline Mat4 scaling(float s) 
{
    return scaling(s,s,s);
}

inline Mat4 perspective(float hfov, float aspect, float near, float far)
{
    Mat4 m;
    float top = near * tan(hfov / 2.0f);
    float right = top * aspect;
    m(0,0) = near / right;
    m(1,1) = near / top;
    m(2,2) = -(far + near) / (far - near);
    m(2,3) = -2*far*near / (far - near);
    m(3,2) = -1.0f;
    return m;
}

}

#endif
