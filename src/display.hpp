/*
 * =====================================================================================
 *
 *       Filename:  display.hpp
 *
 *    Description:  
 *
 *        Version:  1.0
 *        Created:  24.09.2018 18:20:41
 *       Revision:  none
 *       Compiler:  gcc
 *
 *   Organization:
 *
 * =====================================================================================
 */

#include <string>
#include <cstring>

#include <SDL2/SDL.h>

#include "rasterizer.hpp"
#include "framebuffer.hpp"
#include "image.hpp"
class Display
{
    SDL_Window *win = NULL;
    SDL_Renderer *renderer = NULL;
    SDL_Texture *tex = NULL;

    Framebuffer fbo;
    int win_width;
    int win_height;
public:
    Display(unsigned width, unsigned height, const std::string &window_title) :
        win_width(width), win_height(height), fbo(width, height)
    {
        //SDL_Init(SDL_INIT_EVERYTHING);
        SDL_Init(SDL_INIT_EVERYTHING);

        unsigned flags = SDL_WINDOW_RESIZABLE | SDL_WINDOW_ALLOW_HIGHDPI;
        win = SDL_CreateWindow(window_title.c_str(),
                SDL_WINDOWPOS_UNDEFINED,
                SDL_WINDOWPOS_UNDEFINED,
                width, height,
                flags
                );

        renderer = SDL_CreateRenderer(win, -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);
        tex = SDL_CreateTexture(
                renderer,
                SDL_PIXELFORMAT_RGB24,
                SDL_TEXTUREACCESS_STREAMING,
                width,
                height
        );
    }

    ~Display()
    {
        SDL_DestroyWindow(win);
        SDL_Quit();
    }

    void Update()
    {
        SRGBImage rgb = image_a2s(fbo.getImage());
        const uchar* pixels = rgb.getRawData();
        SDL_RenderClear(renderer);
        SDL_UpdateTexture(tex, NULL, pixels, 3 * win_width);
        SDL_RenderCopy(renderer, tex, NULL, NULL);
        SDL_RenderPresent(renderer);
        //SDL_UpdateWindowSurface(win);
    }

    Framebuffer& getFBO() { return fbo; }
};

