/*
 * =====================================================================================
 *
 *       Filename:  lighting.hpp
 *
 *    Description:  Lighting models for framebuffers with deferred shading
 *
 *        Version:  1.0
 *        Created:  29.09.2018 19:43:16
 *       Revision:  none
 *       Compiler:  gcc
 *
 *         Author:  Fedor Novikov (rstar000)
 *   Organization
 *
 * =====================================================================================
 */

#ifndef LIGHT_HPP
#define LIGHT_HPP
#include <algorithm>

#include "math/vector.hpp"
#include "math/matrix.hpp"

#include "framebuffer.hpp"
#include "shader.hpp"

using namespace tmath;

struct PointLight
{
    Vec3 pos;
    RGBAColor color;
    float strength;
};

using FullscreenShader = std::function<void(Framebuffer&, Framebuffer&, UniformVec&)>;

void phong(const PointLight &light, const Vec3 &cam_pos, Framebuffer &input, Framebuffer &output)
{
    int w = input.getWidth();
    int h = input.getHeight();

    auto &attrs = input.getAttribs();
    auto &depth = input.getDepth();
    auto &stencil = input.getStencil();
    auto &image = input.getImage();
    for(int y = 0; y < h; y++) {
        for(int x = 0; x < w; x++) {
            if(!stencil(y,x)) continue;

            auto &cur_attr = attrs(y,x);
            Vec3 light_dir = normalize(cur_attr.pos - light.pos);
            Vec3 view_dir = normalize(cur_attr.pos - cam_pos);
            Vec3 r = reflect(light_dir, cur_attr.normal);

            float falloff = length(cur_attr.pos - light.pos) / light.strength;
            falloff = 1.0f - std::min(falloff, 1.0f);
            falloff = falloff * falloff;
            float energy = dot(-light_dir, cur_attr.normal);
            float spec = pow(dot(-view_dir, r), 20);
            energy = std::min(energy + spec, 1.0f);
            energy = std::max(energy, 0.2f);
            energy *= falloff;
            const auto &color = image.getPixel(x,y);
            Vec4 res_color = energy * color;
            res_color[3] = 1.0f;
            output.putPixel(x,y,depth(y,x), res_color);
        }
    }
}


#endif
