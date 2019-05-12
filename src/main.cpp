#include <mutex>
#include <thread>
#include <vector>

#include <SDL2/SDL.h>

#include "ray.h"
#include "sphere.h"
#include "hitable_list.h"
#include "camera.h"
#include "material.h"


using namespace std;


void worker(bool* kill, int tc, int id, int nx, int ny, int ns, vector<vec3>* samples, unsigned int* pixels, hitable* world, camera* cam);


void show_window(int w, int h, unsigned int *pixels)
{
    // Initialize SDL.
    if (SDL_Init(SDL_INIT_VIDEO) < 0)
        return ;
    
    SDL_Window* win = SDL_CreateWindow("pRat", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, w, h, SDL_WINDOW_ALLOW_HIGHDPI);
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
        this_thread::sleep_for(chrono::milliseconds(20));
    }
    
    // free all resources
    SDL_DestroyTexture(img);
    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(win);
    SDL_Quit();
}


vec3 color(const ray& r, hitable* world, int depth)
{
    hit_record rec;
    if (world->hit(r, 0.001, MAXFLOAT, rec))
    {
        ray scattered;
        vec3 attenuation;
        
        if (depth < 50 && rec.mat_ptr->scatter(r, rec, attenuation, scattered))
            return attenuation * color(scattered, world, depth + 1);
        else
            return vec3(0, 0, 0);
    }
    else
    {
        vec3 unit_direction = unit_vector(r.direction());
        float t = 0.5 * (unit_direction.y() + 1.0);
        return (1.0 - t) * vec3(1.0, 1.0, 1.0) + t * vec3(0.5, 0.7, 1.0);
    }
}


int main()
{
    int nx = 1024;
    int ny = 512;
    int ns = 1000;
    
    vector<vec3> samples;
    samples.resize(nx * ny);
    unsigned int *pixels = new unsigned int[nx * ny];

    hitable* list[4];
    
    list[0] = new sphere(vec3(0, 0, -1), 0.5,  new lambertian(vec3(0.5, 0.5, 0.5)));
    list[1] = new sphere(vec3(0, -100.5, -1), 100, new lambertian(vec3(0.5, 0.5, 0.5)));
    list[2] = new sphere(vec3(1, 0, -1), 0.5, new metal(vec3(0.8, 0.8, 0.8)));
    list[3] = new sphere(vec3(-1, 0, -1), 0.5, new metal(vec3(0.8, 0.8, 0.8)));
    
    hitable* world = new hitable_list(list, 4);
    camera* cam = new camera();
        
    // Spawning threads
    bool kill = false;
    int threadCount = 1;
    threadCount = thread::hardware_concurrency(); // Enable Multithreading
    thread* threads = new thread[threadCount];
    for (int id=0; id<threadCount; id++)
        threads[id] = thread(worker, &kill, threadCount, id, nx, ny, ns, &samples, pixels, world, cam);

    // Wait until the window is closed
    show_window(nx, ny, pixels);
    
    // Terminate threads
    kill = true;
    for (int id=0; id<threadCount; id++)
        threads[id].join();
    
    return EXIT_SUCCESS;
}


void worker(bool* kill, int tc, int id, int nx, int ny, int ns, vector<vec3>* samples, unsigned int* pixels, hitable* world, camera* cam)
{
    int ny1 = ny / tc * (++id);
    int ny2 = ny1 - (ny / tc);
    
    for (int s=0; s < ns; s++)
    {
        for (int j = ny1-1; j >= ny2; j--)
        {
            for (int i=0; i < nx; i++)
            {
                if (*kill)
                    return;
                
                int idx = nx * (ny-j-1) + i;
                
                float u = float(i + drand48()) / float(nx);
                float v = float(j + drand48()) / float(ny);

                ray r = cam->get_ray(u, v);
                vec3 col = color(r, world, 0);
                samples->at(idx) += col;
                
                if (s > 0)
                    col = samples->at(idx) / (s+1);
                
                // Approximte sRGB
                col = vec3(sqrt(col[0]), sqrt(col[1]), sqrt(col[2]));

                // Converting to integers
                int ir = int(255.99 * col[0]);
                int ig = int(255.99 * col[1]);
                int ib = int(255.99 * col[2]);

                lock_guard<mutex> lock(mutex);
                pixels[idx] = (ir << 16) + (ig << 8) + ib;
            }
        }
    }
}
