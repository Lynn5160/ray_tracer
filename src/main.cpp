#include <SDL2/SDL.h>

void show_window(int w, int h, unsigned int *pixels)
{
    // Initialize SDL.
    if (SDL_Init(SDL_INIT_VIDEO) < 0)
        return ;
    
    SDL_Window* win = SDL_CreateWindow("Ray Tracer", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, w, h, SDL_WINDOW_ALLOW_HIGHDPI);
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

int main()
{
    int width = 512;
    int height = 512;
    
    unsigned int *pixels = new unsigned int[width*height];
    
    for (int j = height-1; j >= 0; j--)
    {
        for (int i=0; i < width; i++)
        {
            float r = 1.0;
            float g = 0.0;
            float b = 0.0;
            
            // Converting to integers
            int ir = int(255.99 * r);
            int ig = int(255.99 * g);
            int ib = int(255.99 * b);
            
            int idx = width * (height-j-1) + i;
            pixels[idx] = (ir << 16) + (ig << 8) + ib;
        }
    }
    
    // Wait until the window is closed
    show_window(width, height, pixels);
    
    return EXIT_SUCCESS;
}



