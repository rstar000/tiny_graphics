/*
 * =====================================================================================
 *
 *       Filename:  color.hpp
 *
 *    Description:  Basic data types for color manipulation
 *
 *        Version:  1.0
 *        Created:  15.09.2018 18:30:20
 *       Revision:  none
 *       Compiler:  gcc
 *
 *         Author:  Fedor Novikov (rstar000),
 *
 *
 * =====================================================================================
 */

#ifndef COLOR_HPP
#define COLOR_HPP

#include <vector>
#include <cmath>
#include <variant>

#include "math/vector.hpp"

enum RGBAChannels {
    R = 0,
    G = 1,
    B = 2,
    A = 3
};

enum HSVChannels {
    H = 0,
    S = 1,
    V = 2
};

using RGBAColor = tmath::Vec4;
using RGBColor = tmath::Vec3;
using HSVColor = tmath::Vec3;

HSVColor rgb2hsv(const RGBColor &rgb);


RGBColor hsv2rgb(const HSVColor &in);

RGBColor rgba2rgb(const RGBAColor &rgba);

RGBAColor rgb2rgba(const RGBColor &rgb, float alpha = 1.0f);


/* Color Palletes */



/* Blending */

#endif
