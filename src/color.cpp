/*
 * =====================================================================================
 *
 *       Filename:  color.cpp
 *
 *    Description:  Implementation of color functions
 *
 *        Version:  1.0
 *        Created:  16.09.2018 14:08:40
 *       Revision:  none
 *       Compiler:  gcc
 *
 *         Author: Fedor Novikov (rstar000)
 *   Organization:
 *
 * =====================================================================================
 */

#include "color.hpp"


HSVColor rgb2hsv(const RGBColor &rgb)
{
    HSVColor out;
    double      min, max, delta;

    min = rgb[R] < rgb[G] ? rgb[R] : rgb[G];
    min = min < rgb[B] ? min  : rgb[B];

    max = rgb[R] > rgb[G] ? rgb[R] : rgb[G];
    max = max  > rgb[B] ? max  : rgb[B];

    out[V] = max;                                // v
    delta = max - min;
    if(delta < 0.00001)
    {
        out[S] = 0;
        out[H] = 0; // undefined, maybe nan?
        return out;
    }
    if(max > 0.0) { // NOTE: if Max is == 0, this divide would cause a crash
        out[S] = (delta / max);                  // s
    } else {
        out[S] = 0.0;
        out[H] = NAN;                            // its now undefined
        return out;
    }
    if(rgb[R] >= max)                           // > is bogus, just keeps compilor happy
        out[H] = (rgb[G] - rgb[B]) / delta;        // between yellow & magenta
    else
    if(rgb[G] >= max)
        out[H] = 2.0 + (rgb[B] - rgb[R]) / delta;  // between cyan & yellow
    else
        out[H] = 4.0 + (rgb[R] - rgb[G]) / delta;  // between magenta & cyan

    out[H] *= 60.0;                              // degrees

    if(out[H] < 0.0)
        out[H] += 360.0;

    return out;
}

RGBColor hsv2rgb(const HSVColor &in)
{
    double      hh, p, q, t, ff;
    long        i;
    RGBColor         out;

    if(in[S] <= 0.0) {       // < is bogus, just shuts up warnin[S]
        out[R] = in[V];
        out[G] = in[V];
        out[B] = in[V];
        return out;
    }
    hh = in[H];
    if(hh >= 360.0) hh = 0.0;
    hh /= 60.0;
    i = (long)hh;
    ff = hh - i;
    p = in[V] * (1.0 - in[S]);
    q = in[V] * (1.0 - (in[S] * ff));
    t = in[V] * (1.0 - (in[S] * (1.0 - ff)));

    switch(i) {
    case 0:
        out[R] = in[V];
        out[G] = t;
        out[B] = p;
        break;
    case 1:
        out[R] = q;
        out[G] = in[V];
        out[B] = p;
        break;
    case 2:
        out[R] = p;
        out[G] = in[V];
        out[B] = t;
        break;

    case 3:
        out[R] = p;
        out[G] = q;
        out[B] = in[V];
        break;
    case 4:
        out[R] = t;
        out[G] = p;
        out[B] = in[V];
        break;
    case 5:
    default:
        out[R] = in[V];
        out[G] = p;
        out[B] = q;
        break;
    }
    return out;
}

RGBColor rgba2rgb(const RGBAColor &rgba)
{
    return RGBColor({rgba[R], rgba[G], rgba[B]});
}

RGBAColor rgb2rgba(const RGBColor &rgb, float alpha)
{
    return RGBAColor({rgb[R], rgb[G], rgb[B], alpha});
}


