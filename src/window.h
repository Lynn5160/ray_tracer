#ifndef WINDOW
#define WINDOW

#include <SDL2/SDL.h>

void show_window(int w, int h, unsigned int *pixels)
{
    // Initialize SDL.
    if (SDL_Init(SDL_INIT_VIDEO) < 0)
        return ;
    
    SDL_Window* win = SDL_CreateWindow("Ray Tracing", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, w, h, SDL_WINDOW_ALLOW_HIGHDPI);
    SDL_Renderer* renderer = SDL_CreateRenderer(win, -1, SDL_RENDERER_ACCELERATED);
    SDL_Texture* img = SDL_CreateTexture(renderer, SDL_PIXELFORMAT_ARGB8888, SDL_TEXTUREACCESS_STATIC, w, h);
    
    int pitch = w * sizeof(unsigned int);
    SDL_Event event;
    while (true)
    {
        if (SDL_PollEvent(&event))
            if (SDL_QUIT == event.type)
                break;
        
        SDL_UpdateTexture(img, NULL, pixels, pitch);
        SDL_RenderCopy(renderer, img, NULL, NULL);
        SDL_RenderPresent(renderer);
        SDL_Delay(100);
    }
    
    // free all resources
    SDL_DestroyTexture(img);
    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(win);
    SDL_Quit();
}

#endif