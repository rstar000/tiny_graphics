/*
 * =====================================================================================
 *
 *       Filename:  image.cpp
 *
 *    Description:  Implementation for some image functions
 *
 *        Version:  1.0
 *        Created:  16.09.2018 13:59:37
 *       Revision:  none
 *       Compiler:  gcc
 *
 *         Author:  Fedor Novikov (rstar000)
 *   Organization:
 *
 * =====================================================================================
 */

#include "image.hpp"

#define STB_IMAGE_WRITE_IMPLEMENTATION
#include "stb_image_write.h"

void save_image(const SRGBImage &img, const std::string &filename)
{
    int ok = stbi_write_png(filename.c_str(), img.getWidth(), img.getHeight(), 3, img.getRawData(), 0);
    if(!ok) {
        std::cout << "Can't save image: " << filename << std::endl;
    }
}

SRGBImage image_a2s(const RGBAImage &src)
{
    int w = src.getWidth();
    int h = src.getHeight();

    SRGBImage res(w, h);
    for(int y = 0; y < h; y++) {
        for(int x = 0; x < w; x++) {
            for(int c = 0; c < 3; c++) {
                float val = src.value_at(y,x)[c];
                res.at(y,x,c) = val * 255;
            }
        }
    }

    return res;
}

RGBAImage image_s2a(const SRGBImage &src)
{
    int w = src.getWidth();
    int h = src.getHeight();

    RGBAImage res(w, h);
    for(int y = 0; y < h; y++) {
        for(int x = 0; x < w; x++) {
            for(int c = 0; c < 3; c++) {
                res.at(y,x)[c] = src.value_at(y,x,c);
            }

            res.at(y,x)[3] = 1.0f;
        }
    }

    return res;
}

