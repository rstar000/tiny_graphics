/*
 * =====================================================================================
 *
 *       Filename:  framebuffer.hpp
 *
 *    Description:  Framebuffer class for storing image data.
 *
 *        Version:  1.0
 *        Created:  15.09.2018 21:37:50
 *       Revision:  none
 *       Compiler:  gcc
 *
 *         Author:  Fedor Novikov (rstar000)
 *   Organization:
 *
 * =====================================================================================
 */

#ifndef FRAMEBUFFER_HPP
#define FRAMEBUFFER_HPP

#include "image.hpp"

using tmath::Vec3;


struct FragAttrib
{
    Vec3 pos;
    Vec3 normal;
};

template<typename AttribT>
class AbstractFramebuffer
{
    int width, height;

    RGBAImage image;
    DepthMap depth;
    StencilMap stencil;

    RenderBuffer<AttribT, 1> attribs;

    bool depth_test = true;


public:
    AbstractFramebuffer(int width, int height) :
        width(width), height(height),
        image(width, height),
        attribs(width, height),
        depth(width, height),
        stencil(width, height)
    { }

    void clearColor(RGBAColor init_color)
    {
        image.Fill(init_color);
    }

    void clearDepth(float init_value)
    {
        depth.Fill(init_value);
    }

    void clearStencil(uchar init_value)
    {
        stencil.Fill(init_value);
    }

    void clearAll(RGBAColor c, float d = 100000.0f, uchar s = 0)
    {
        clearColor(c);
        clearDepth(d);
        clearStencil(s);
    }

    bool checkDepth(int y, int x, float test)
    {
        return !depth_test or (test < depth.value_at(y,x));
    }

    void putPixel(int x, int y, float depth_val,
                  const RGBAColor &color)
    {
        if(depth_test) {
            depth(y, x) = depth_val;
        }

        stencil(y,x) = 1;
        image.setPixel(x,y, color);
    }

    void putAttrib(int x, int y, AttribT attr)
    {
        attribs(y,x) = attr;
    }

    void putPixel(const Vec3 &screen_pos, const RGBAColor &color)
    {
        int x = screen_pos[0];
        int y = screen_pos[1];
        float z = screen_pos[2];
        if(depth_test) {
            depth(y,x) = z;
        }
        stencil(y,x) = 1;
        image.setPixel(x, y, color);
    }

    int getWidth() { return width; }
    int getHeight() { return height; }
    void setDepthTest(bool depth_flag) { depth_test = depth_flag; }
    bool depthEnabled() { return depth_test; }
    void Save(const std::string &filename)
    {
        auto rgb = image_a2s(image);
        save_image(rgb, filename);
    }

    RGBAImage &getImage() { return image; }
    DepthMap &getDepth() { return depth; }
    StencilMap &getStencil() { return stencil; }
    RenderBuffer<AttribT, 1> &getAttribs() { return attribs; }
};

using Framebuffer = AbstractFramebuffer<FragAttrib>;
using SimpleFB = AbstractFramebuffer<float>;

inline Framebuffer fb_like(Framebuffer &other)
{
    return Framebuffer(other.getWidth(), other.getHeight());
}


#endif
