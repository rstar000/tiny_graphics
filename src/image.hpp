/*
 * =====================================================================================
 *
 *       Filename:  image.hpp
 *
 *    Description:  A basic containter for an image and a framebuffer
 *
 *        Version:  1.0
 *        Created:  15.09.2018 19:44:54
 *       Revision:  none
 *       Compiler:  gcc
 *
 *         Author:  Fedor Novikov (rstar000)
 *   Organization:
 *
 * =====================================================================================
 */

#ifndef IMAGE_HPP
#define IMAGE_HPP

#include "color.hpp"

using uchar = unsigned char;

template<typename T>
class Array3D
{
protected:
    int width, height;
    int num_channels;
    std::vector<T> values;
public:
    Array3D() { }
    Array3D(int width, int height, int num_channels) :
        width(width), height(height), num_channels(num_channels)
    {
        values.resize(width * height * num_channels);
    }

    Array3D(const Array3D &other) :
        width(other.width),
        height(other.height),
        num_channels(other.num_channels),
        values(other.values)
    { }

    int getWidth() const { return width; }
    int getHeight() const { return height; }
    int getNumChannels() { return num_channels; }

    T& operator() (int y, int x, int ch = 0)
    {
        return values[(y * width + x) * num_channels + ch];
    }

    T& at(int y, int x, int ch = 0)
    {
        return values[(y * width + x) * num_channels + ch];
    }

    T value_at(int y, int x, int ch = 0) const
    {
        return values[(y * width + x) * num_channels + ch];
    }

    const T* getRawData() const
    {
        return values.data();
    }

    void Fill(T value)
    {
        for(int y = 0; y < height; y++) {
            for(int x = 0; x < width; x++) {
                for(int c = 0; c < num_channels; c++) {
                    at(y,x,c) = value;
                }
            }
        }
    }
};

class RGBAImage : public Array3D<RGBAColor>
{
public:
    RGBAImage();
    RGBAImage(int width, int height) :
        Array3D<RGBAColor>(width, height, 1)
    { }
    //Image(const std::string &filename);
    RGBAImage(const RGBAImage &other) : Array3D<RGBAColor>(other) {}

    void Resize(int new_width, int new_height)
    {
        width = new_width;
        height = new_height;
        values.resize(height * width * num_channels);
    }

    //void Save(const std::string &save_path) { // TODO}
    void setPixel(int x, int y, RGBAColor c)
    {
        at(y,x,0) = c;
    }

    RGBAColor getPixel(int x, int y) const {
        return value_at(y,x);
    }
};

class SRGBImage : public Array3D<uchar>
{
public:
    SRGBImage(int width, int height) :
        Array3D<uchar>(width, height, 3)
    { }

    SRGBImage(const RGBAImage &other) :
        Array3D<uchar>(other.getWidth(), other.getHeight(), 3)
    {
        int w = getWidth();
        int h = getHeight();
        for(int y = 0; y < h; y++) {
            for(int x = 0; x < w; x++) {
                for(int c = 0; c < 3; c++) {
                    at(y,x,c) = other.value_at(y,x)[c] * 255;
                }
            }
        }
    }
};


template<typename T, int NUM_CHANNELS>
class RenderBuffer : public Array3D<T>
{
public:
    RenderBuffer(int width, int height) :
        Array3D<T>(width, height, NUM_CHANNELS)
    { }
};

using DepthMap = RenderBuffer<float, 1>;
using StencilMap = RenderBuffer<uchar, 1>;
using NormalMap = RenderBuffer<tmath::Vec3, 1>;
// Convert rgba image to srgb
SRGBImage image_a2s(const RGBAImage &src);

// Convert srgb image to rgba
RGBAImage image_s2a(const SRGBImage &src);

void save_image(const SRGBImage &img, const std::string &filename);

#endif
