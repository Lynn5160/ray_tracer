#include <thread>

#include "window.h"
#include "ray.h"
#include "sphere.h"
#include "hitable_list.h"
#include "camera.h"
#include "material.h"

using namespace std;

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
        float t = 0.5 * (r.direction().y() + 1.0);
        return (1.0 - t) * vec3(1.0, 1.0, 1.0) + t * vec3(0.5, 0.7, 1.0);
    }
}

void worker(bool* kill, int tc, int id, int width, int height, int sampling, vec3* samples, unsigned int* pixels, hitable* world, camera* cam)
{
    vec3 col;
    float u, v;
    int idx, ir, ig, ib;

    int h_max = height / tc * id;
    int h_min = h_max - (height / tc);
    
    for (int s = 1; s < sampling; s++)
    {
        for (int j = h_max-1; j >= h_min; j--)
        {
            for (int i = 0; i < width; i++)
            {
                if (*kill)
                    return;
                
                idx = width * (height-j-1) + i;

                u = float(i + drand48()) / float(width);
                v = float(j + drand48()) / float(height);

                ray r = cam->get_ray(u, v);
                
                samples[idx] += color(r, world, 0);

                col = samples[idx] / s;

                // Apperoximate sRGB and convert to integers
                ir = int(255.99 * sqrt(col[0]));
                ig = int(255.99 * sqrt(col[1]));
                ib = int(255.99 * sqrt(col[2]));

                pixels[idx] = (ir << 16) + (ig << 8) + ib;
            }
        }
    }
}

int main()
{
    int width = 1024;
    int height = 512;
    int sampling = 100;
    
    vec3* samples = new vec3[width * height];
    unsigned int* pixels = new unsigned int[width * height];

    hitable* list[4];
    
    list[0] = new sphere(vec3(0, 0, -1), 0.5,  new diffuse(vec3(0.8, 0.8, 0.8)));
    list[1] = new sphere(vec3(0, -100.5, -1), 100, new diffuse(vec3(0.0, 0.5, 0.0)));
    list[2] = new sphere(vec3(1, 0, -1), 0.5, new metal(vec3(0.8, 0.6, 0.2), 0.35));
    list[3] = new sphere(vec3(-1, 0, -1), 0.5, new metal(vec3(0.8, 0.8, 0.8), 0));
    
    hitable* world = new hitable_list(list, 4);
    camera* cam = new camera();
    
    // Spawning threads
    bool kill = false;
    int threadCount = 16;
    threadCount = thread::hardware_concurrency(); // Enable Multithreading
    thread* threads = new thread[threadCount];
    for (int id=0; id<threadCount; id++)
        threads[id] = thread(worker, &kill, threadCount, id+1, width, height, sampling, samples, pixels, world, cam);

    // Wait until the window is closed
    show_window(width, height, pixels);
    
    // Terminate threads
    kill = true;
    for (int id=0; id<threadCount; id++)
        threads[id].join();
    
    return EXIT_SUCCESS;
}
