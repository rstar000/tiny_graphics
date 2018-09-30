/*
 * =====================================================================================
 *
 *       Filename:  rasterizer.hpp
 *
 *    Description:  Several rasterization methods for drawing triangles
 *
 *        Version:  1.0
 *        Created:  15.09.2018 22:30:16
 *       Revision:  none
 *       Compiler:  gcc
 *
 *         Author:  Fedor Novikov (rstar000)
 *   Organization:
 *
 * =====================================================================================
 */

#ifndef RASTERIZER_HPP
#define RASTERIZER_HPP

#include <array>
#include <algorithm>
#include <functional>

#include "framebuffer.hpp"
#include "shader.hpp"

using tmath::Vec3;

using Triangle = std::tuple<Vertex&, Vertex&, Vertex&>;

struct ScreenPoint
{
    int x, y;
    AttribVec attr;
};

using PointPair = std::pair<Vertex, Vertex>;
using PointVariant = std::variant<std::monostate, Vertex, PointPair>;

struct LineRasterizer
{
private:
    Framebuffer &fb;
    PartialFSH fsh;
public:
    LineRasterizer(Framebuffer &fb, PartialFSH fsh) :
        fb(fb), fsh(fsh)
    { }

    void operator() (PointPair &p)
    {
        int y = p.first.position.y();
        if(y < 0 or y > fb.getHeight()) return;
        int x1 = round(p.first.position[0]);
        int x2 = round(p.second.position[0]);

        int z1 = p.first.position[2];
        int z2 = p.second.position[2];

        if(x2 < 0 or x1 > fb.getWidth() - 1) return;

        int cx1 = std::clamp(x1, 0, fb.getWidth()-1);
        int cx2 = std::clamp(x2, 0, fb.getWidth()-1);
        for(int x = cx1; x <= cx2; x++) {
            float t = float(x - x1) / (x2 - x1);
            float z = (1.0f-t) * z1 + t * z2;

            if(z < -1.0f or z > 1.0f) continue;

            AttribVec cur_attr = interpolate(p.first.attr, p.second.attr, t);
            fb.putPixel(x, y, z, fsh(cur_attr));
        }
    }

    void operator() (Vertex &point)
    {
        int x = point.position[0];
        if(x < 0 or x > fb.getWidth()) return;
        fb.putPixel(point.position, fsh(point.attr));
    }

    void operator() (std::monostate&) { }
};

bool facing_forward(const Triangle &tri);

void rasterize_triangle(Triangle tri, PartialFSH fsh, Framebuffer &fb);

#endif
