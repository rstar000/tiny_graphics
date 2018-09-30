#ifndef SDF_HPP
#define SDF_HPP

#include <cmath>
#include <iostream>
#include "math/vector.hpp"
#include "ray_marching.hpp"

using namespace tmath;

inline Vec3 vec_power(const Vec3 &v, int n)
{
    float r = length(v);
    float phi = atan2(v[1], v[0]);
    float theta = acos(v[2] / r);


    float rn = pow(r,n);
    float sp = sin(n * phi);
    float st = sin(n * theta);
    float cp = cos(n * phi);
    float ct = cos(n * theta);

    Vec3 res = rn * Vec3({st*cp, st*sp, ct});
    return res;
}

inline float mandelbulbDE(Vec3 pos, int n)
{
    Vec3 z = pos;
    float dr = 1.0f;
    float r = 0.00f;
    int num_iter = 15;
    for(int i = 0; i < num_iter; i++) {
        r = length(z);
        if(r > 1.15) break;
        z = vec_power(z, n) + pos;
		dr =  pow( r, n-1.0)*n*dr + 1.0;
    }
    float res = 0.5 * log(r) * r / dr;
    if(res < 0) return 1.0f;
    return res;
}

#endif
