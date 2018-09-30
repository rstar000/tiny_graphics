/*
 * =====================================================================================
 *
 *       Filename:  rasterizer.cpp
 *
 *    Description:  Implementation of rasterizer
 *
 *        Version:  1.0
 *        Created:  16.09.2018 16:01:22
 *       Revision:  none
 *       Compiler:  gcc
 *
 *         Author:  Fedor Novikov (rstar000)
 *   Organization:
 *
 * =====================================================================================
 */


#include "rasterizer.hpp"
#include <optional>
#include <iostream>

using tmath::Vec3;

Vertex vertex2screen(Vertex &v, int w, int h) {
    float cx = v.position[0];
    float cy = v.position[1];
    Vec3 pos({
            (cx+1.0f) * 0.5f * w,
            (1.0f-cy) * 0.5f * h,
            v.position[2]
            });

    return Vertex({pos , v.attr});
}


void rasterize_triangle(Triangle tri, PartialFSH fsh, Framebuffer &fb)
{
    int w = fb.getWidth();
    int h = fb.getHeight();

    std::array<Vertex, 3> points = {
        vertex2screen(std::get<0>(tri), w, h),
        vertex2screen(std::get<1>(tri), w, h),
        vertex2screen(std::get<2>(tri), w, h)
    };

    auto intersect_line = [&points](int y) -> PointVariant
    {
        int num_int = 0;
        std::array<Vertex, 3> res_verts;

        for(int i = 0; i < 3; i++) {
            auto &v1 = points[i];
            auto &v2 = points[(i+1) % 3];
            auto &p1 = v1.position;
            auto &p2 = v2.position;

            if((p1.y() - y) * (p2.y() - y) > 0) continue;
            // x = (x1-x2)y - x1y2 + x2y1 / (y1 - y2)
            float t = (y - p1.y()) / (p2.y() - p1.y());
            if(t < 0.001) continue;

            res_verts[num_int] = interpolate(v1, v2, t);
            res_verts[num_int].position[1] = y;
            num_int++;
            // Edge condition: intersection with point
        }

        if(num_int == 2) {
            if(res_verts[0].position.x() > res_verts[1].position.x()) {
                return std::make_pair(res_verts[1], res_verts[0]);
            } else {
                return std::make_pair(res_verts[0], res_verts[1]);
            }
        } else if(num_int == 1) {
            return res_verts[0];
        }

        return std::monostate();
    };

    std::sort(points.begin(), points.end(), [](Vertex &p1, Vertex &p2) {
                return p1.position[1] < p2.position[1];
            });

    


    int y_begin = points[0].position.y();
    int y_end = points[2].position.y();

    LineRasterizer rast(fb, fsh);
    y_begin = std::clamp(y_begin, 0, h-1);
    y_end = std::clamp(y_end, 0, h-1);
    for(int y = y_begin; y < y_end; y++) {
        PointVariant inter = intersect_line(y);
        std::visit(rast, inter);
    }
}
