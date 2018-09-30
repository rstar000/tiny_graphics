// This is a part of Tiny Math library.
// Simple vector operations implemented using functional patterns
//

#ifndef VECTOR_HPP
#define VECTOR_HPP


#include <cmath>
#include <iostream>
#include <vector>
#include <array>
#include <cassert>
#include <functional>
#include <algorithm>
#include <initializer_list>

namespace tmath
{
/* Prototypes */
template<int N>
class Vector;

template <int N>
float length(const Vector<N>& vec);

template<int N>
Vector<N> min(const Vector<N> &left, const Vector<N> &right);

template<int N>
Vector<N> max(const Vector<N> &left, const Vector<N> &right);



/**
 * @brief The Vector class
 * This template class is a container for an N-dimentional vector.
 * It has all the usual operators and functions you expect in glsl.
 * */
template <int N>
class Vector
{
    std::array<float, N> value;

public:
    Vector()
    {
        std::fill(value.begin(), value.end(), 0.0f);
    }

    Vector(const std::array<float, N> &ini)
    {
        value = ini;
    }

    Vector(const std::array<float, N> &&ini) :
        value(std::move(ini))
    { }

    Vector(const Vector &other) :
        value(other.value)
    { }

    Vector(const Vector &&other) :
        value(std::move(other.value))
    { }

    Vector& operator= (Vector& other)
    {
        value = other.value;
    }

    Vector& operator= (Vector&& other)
    {
        value = std::move(other.value);
    }

    float& operator[] (int idx)
    {
        return value[idx];
    }

    float operator[] (int idx) const
    {
        return value[idx];
    }

    static Vector binaryOp(const Vector &left, const Vector &right, std::function<float(float, float)> op)
    {
        Vector<N> res;
        std::transform(left.value.begin(), left.value.end(),
                        right.value.begin(),
                        res.value.begin(),
                        op);
        return res;
    }

    static Vector unaryOp(const Vector &vec, std::function<float(float)> op)
    {
        Vector<N> res;
        std::transform(vec.value.begin(), vec.value.end(), res.value.begin(), op);
        return res;

    }

    Vector apply(std::function<float(float)> op) const { return unaryOp(*this, op); }

    static float reduceLeft(const Vector &vec, std::function<float(float, float)> reduceOp, float ini)
    {
        float res = ini;
        for(int i = 0; i < N; i++) {
            res = reduceOp(res, vec[i]);
        }

        return res;
    }

    Vector operator+ (const Vector &other) const
    {
        return binaryOp(*this, other, [](float x, float y) { return x+y; });
    }

    Vector operator- (const Vector &other) const
    {
        return binaryOp(*this, other, [](float x, float y) { return x-y; });
    }

    Vector operator-() const
    {
        return apply([](float x) { return -x; });
    }

    float& x() { return value[0]; }
    float x() const { return value[0]; }

    float& y() { return value[1]; }
    float y() const { return value[1]; }

    float& z() { return value[2]; }
    float z() const { return value[2]; }
};


typedef Vector<3> Vec3;
typedef Vector<4> Vec4;

inline Vec4 toVec4(const Vec3 &v3, float w)
{
    return Vec4({v3[0], v3[1], v3[2], w});
}

inline Vec3 toVec3(const Vec4 &v4)
{
    return Vec3({v4[0], v4[1], v4[2]});
}
template<int N>
std::ostream& operator<<(std::ostream &os, const Vector<N> &vec)
{
    os << "Vector<" << N << "> [";
    for(int i = 0; i < N; i++) {
        os << vec[i];
        if(i != N-1) os << ", ";
    }
    os << ']';
    return os;
}

/**
 * @brief dot
 * Computes the dot product of two vectors
 * */

template<int N>
float sum(const Vector<N> &vec)
{
    return Vector<N>::reduceLeft(vec, [](float a, float b) { return a + b; }, 0.0f);
}

template <int N>
inline float dot(const Vector<N> &left, const Vector<N> &right)
{
    float res = 0.0f;
    for(int i = 0; i < N; i++) {
        res += left[i] * right[i];
    }
    return res;

    // Beautiful but slow solution.
    //return sum(Vector<N>::binaryOp(left, right, [](float x, float y) { return x*y; }));

}

template<int N>
inline float length2(const Vector<N> &vec)
{
    return dot(vec, vec);
}

template<int N>
inline Vector<N> operator* (float m, const Vector<N> &vec)
{
    return vec.apply([m](float x) { return x * m; });
}

template<int N>
inline Vector<N> operator* (const Vector<N> &vec, float m)
{
    return m * vec;
}


template<int N>
inline Vector<N> operator/ (const Vector<N> &vec, float m)
{
    return (1.0 / m) * vec;
}

template<int N>
inline Vector<N> min(const Vector<N> &left, const Vector<N> &right)
{
    return Vector<N>::binaryOp(left, right, [](float x, float y) -> float {return std::min(x,y);});
}


template<int N>
inline Vector<N> max(const Vector<N> &left, const Vector<N> &right)
{
    return Vector<N>::binaryOp(left, right, [](float x, float y) -> float {return std::max(x,y);});
}


template<int N>
inline Vector<N> abs(const Vector<N> &vec)
{
    return vec.apply(fabs);
}


template<int N>
inline Vector<N> normalize(const Vector<N> &vec)
{
    float len = length(vec);
    return vec.apply([len](float x) { return x / len; });
}


template<int N>
inline float distance(const Vector<N> &left, const Vector<N> &right)
{
    return length(left - right);
}

/**
 * @brief length
 * Computes euclidean legth of the vector
 * */
template <int N>
inline float length(const Vector<N>& vec)
{
    return float(sqrt(dot(vec, vec)));
}

template<int N>
inline Vector<N> clamp(Vector<N> &x, Vector<N> &min_vec, Vector<N> &max_vec)
{
    return min(max_vec, max(min_vec, x));
}

inline Vec3 cross(const Vec3 &left, const Vec3 &right)
{
    float x = left[1] * right[2] - left[2] * right[1];
    float y = left[2] * right[0] - left[0] * right[2];
    float z = left[0] * right[1] - left[1] * right[0];
    return Vec3({x,y,z});
}

template<int N>
inline Vector<N> interpolate(const Vector<N> &a, const Vector<N> &b, float t)
{
    return (1.0 - t) * a + t * b;
}

template<int N>
inline Vector<N> reflect(const Vector<N> &i, const Vector<N> &n)
{
    return i - 2.0f * dot(n, i) * n;
}

}


#endif
