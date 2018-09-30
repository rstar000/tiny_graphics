/*
 * =====================================================================================
 *
 *       Filename:  ray_marching.hpp
 *
 *    Description:  Base function for ray marching
 *
 *        Version:  1.0
 *        Created:  29.09.2018 16:51:27
 *       Revision:  none
 *       Compiler:  gcc
 *
 *         Author:  Fedor Novikov (rstar000)
 *   Organization:
 *
 * =====================================================================================
 */

#ifndef RAYMARCHING_HPP
#define RAYMARCHING_HPP

#include <functional>
#include <optional>

#include "draw.hpp"
#include "math/vector.hpp"
#include "math/matrix.hpp"
#include "math/transform.hpp"
#include "math/quaternion.hpp"
#include "color.hpp"

using namespace tmath;

struct TraceResult
{
    RGBAColor color;
    Vec3 normal;
    float depth;
};

using MaybeResult = std::optional<TraceResult>;

using TraceFunc = std::function<MaybeResult(const Vec3 &, const Vec3 &)>;
using DistanceFunc = std::function<float(Vec3)>;


inline Vec3 sdf_normal(DistanceFunc d, Vec3 point)
{
    Vec3 res;
    for(int i = 0; i < 3; i++) {
        Vec3 delta;
        delta[i] = 0.001;
        res[i] = d(point + delta) - d(point - delta);
    }
    return normalize(res);
}

inline void trace(Framebuffer &fb, TraceFunc f, float fov, const Vec3 &pos, const Quat &rot)
{
    int w = fb.getWidth();
    int h = fb.getHeight();
    float aspect = float(w) / h;
    #pragma omp parallel for num_threads(4)
    for(int y = 0; y < h; y++) {
        for(int x = 0; x < w; x++) {
            Vec3 origin = pos;
            float u = 2 * float(x) / w - 1.0f;
            float v = 2 * float(y) / h - 1.0f;

            Quat q1 = angle_axis(fov * u / 2, Vec3({0,1,0}));
            Quat q2 = angle_axis(fov * v / (2*aspect), Vec3({1,0,0}));
            Vec3 dir({0,0,1});

            dir = rotate(rotate(dir, q1), q2);
            dir = rotate(dir, rot);

            auto res = f(origin, dir);
            if(res) {
                TraceResult &tr = *res;
                fb.putPixel(x,y,tr.depth,tr.color);
                Vec3 pos = origin + tr.depth * dir;
                FragAttrib attr = {pos, tr.normal};
                fb.putAttrib(x,y,attr);
            }
        }
    }
}

#endif
