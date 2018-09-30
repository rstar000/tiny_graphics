/*
 * =====================================================================================
 *
 *       Filename:  main.cpp
 *
 *    Description:  
 *
 *        Version:  1.0
 *        Created:  17.09.2018 00:24:20
 *       Revision:  none
 *       Compiler:  gcc
 *
 *         Author:  YOUR NAME (), 
 *   Organization:  
 *
 * =====================================================================================
 */

#include <iostream>
#include <thread>
#include <chrono>
#include <iomanip>

#include "shader.hpp"
#include "rasterizer.hpp"
#include "draw.hpp"
#include "math/transform.hpp"
#include "display.hpp"
#include "ray_marching.hpp"
#include "lighting.hpp"
#include "sdf.hpp"
using tmath::Vec3;
using tmath::Vec4;
using tmath::Mat4;

using FragmentShader = std::function<Vec4(const AttribVec &input,
                                          const UniformVec &uniforms
                                         )>;

Vertex vsh(const AttribVec &attrs, const UniformVec &unis)
{
    Mat4 M = std::get<Mat4>(unis[0]);
    Mat4 P = std::get<Mat4>(unis[1]);
    Vec3 pos = std::get<Vec3>(attrs[0]);
    Vec4 pos4 = tmath::toVec4(pos, 1.0f);
    pos4 = P * (M * pos4);
    pos = tmath::toVec3(pos4) / pos4[3];
    return Vertex({pos, {attrs[1]}});
}

Vec4 fsh(const AttribVec &inp, const UniformVec &unis)
{
    Vec3 color = std::get<Vec3>(inp[0]);
    return toVec4(color, 1.0f);
}

Model create_triangle(const std::array<Vec3, 3> &points)
{
    Model m;
    m.indeces = { {0,1,2} };
    m.v_attrs.push_back({ points[0], Vec3({1.0 ,0.0, 0.0}) });
    m.v_attrs.push_back({ points[1], Vec3({0.0 ,1.0, 0.0}) });
    m.v_attrs.push_back({ points[2], Vec3({0.0 ,0.0, 1.0}) });
    return m;
}


float DE(Vec3 z)
{
    z[0] = std::fmod(abs(z[0]) + 0.3, 0.6) - 0.3;
    z[2] = std::fmod(abs(z[2]) + 0.3, 0.6) - 0.3;
    return length(z)-0.05;             // sphere DE
}

float spheres_and_fractal(Vec3 z)
{
//    return mandelbulbDE(z, 8);
    return std::min(4.0f, mandelbulbDE(z,8));
}

MaybeResult trace_f(const Vec3 &origin, const Vec3 &direction)
{
    float d = 0.0f;
    int max_steps = 25;
    float epsilon = 0.0001;
    for(int i = 0; i < max_steps; i++) {
        Vec3 pos = origin + d * direction;
        float dist = spheres_and_fractal(pos);
        if(dist < epsilon)
        {
            Vec3 normal = sdf_normal(spheres_and_fractal, pos);
            Vec4 color = toVec4(abs(normal), 1.0f);
            return TraceResult({color, normal, d});
        }
        d += dist;
    }

    return std::nullopt;
}


int main()
{
    Model tri = create_triangle({
            Vec3({-0.5f, -0.5f, 0.0f}),
            Vec3({0.5f, -0.5f, 0.0f}),
            Vec3({0.9f, 0.5f, 0.0f})});
    Shader sh = { vsh, fsh };
    Framebuffer fb(3840, 2160);
    fb.clearAll(RGBAColor({1,0,0,1}));


    float pi = 3.1415;
    Mat4 proj = tmath::perspective(pi/4, 16.0/9, 0.01, 100);
    Mat4 model_transform = tmath::eye();
    Mat4 translate = tmath::translation(Vec3({0,0,3}));
    //float angle = 0.0f;
    int frame = 0;

    float cam_fov = pi / 4;
    int num_frames = 120;

    Vec3 light_pos = Vec3({1,2,-4});
    PointLight light;
    light.pos = light_pos;
    light.strength = 45;

//    for(int i = 0; i < num_frames; i++) {
    int i = 75;
    fb.clearAll(RGBAColor({0,0,0,1}));
        float t = float(i) / num_frames;
        float angle = t * 2 * pi;

        Vec3 cam_pos({5 * cos(angle),1.0f,5 * sin(angle)});
        Quat cam_dir = look_at(cam_pos, Vec3({0,0,0}));
        light.pos = cam_pos;
        trace(fb, trace_f, cam_fov, cam_pos, cam_dir);

        auto fb2 = fb_like(fb);
        phong(light, cam_pos, fb, fb2);

        std::ostringstream ss;
        ss << "img/" << std::setw(5) << std::setfill('0');
        ss  << "4k.png";

        std::string filename = ss.str();
        fb2.Save(filename);

        std::cout << filename << " saved!\n";

    //}

    return 0;

    Display dis(3840, 2160, "Main window");
    while(true) {
        SDL_PumpEvents();
        SDL_Event e;
        if (SDL_PollEvent(&e)) {
            if (e.type == SDL_QUIT) {
                break;
            }
        }
        //Mat4 rot = tmath::rotation(angle, Vec3({0,1,0}));
        ///model_transform = translate * rot;
        //angle += 0.05;
        RGBAColor clear({0,0,0,1});
        dis.getFBO().clearAll(clear);
        //UniformVec unis = { model_transform, proj };
        //draw_model(tri, sh, dis.getFBO(), unis);
        dis.Update();
        frame++;

        std::this_thread::sleep_for(std::chrono::milliseconds(16));
    }

    return 0;
}
