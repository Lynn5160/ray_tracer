#include <thread>

#include "window.h"
#include "vec3.h"
#include "ray.h"
#include "sphere.h"
#include "hitable.h"
#include "hitable_list.h"
#include "camera.h"

using namespace std;

vec3 color(const ray& r, hitable* world)
{
    hit_record rec;
    if (world->hit(r, 0.0, MAXFLOAT, rec))
        return 0.5 * vec3(rec.normal.x() + 1, rec.normal.y() + 1, rec.normal.z() + 1);
    else
    {
        float t = 0.5 * (r.direction().y() + 1.0);
        return (1.0 - t) * vec3(1.0, 1.0, 1.0) + t * vec3(0.5, 0.7, 1.0);
    }
}

void worker(bool* kill, int tc, int id, int nx, int ny, int ns, unsigned int* pixels, hitable* world, camera* cam)
{
    int ny1 = ny / tc * (++id);
    int ny2 = ny1 - (ny / tc);
    
    for (int j = ny1-1; j >= ny2; j--)
    {
        for (int i=0; i < nx; i++)
        {
            if (*kill)
                return;

            vec3 col(0, 0, 0);
            for(int s=0; s < ns; s++)
            {
                float u = float(i + drand48()) / float(nx);
                float v = float(j + drand48()) / float(ny);

                ray r = cam->get_ray(u, v);
                col += color(r, world);
            }

            col /= float(ns);

            // Converting to integers
            int ir = int(255.99 * col[0]);
            int ig = int(255.99 * col[1]);
            int ib = int(255.99 * col[2]);

            int idx = nx * (ny-j-1) + i;
            pixels[idx] = (ir << 16) + (ig << 8) + ib;
        }
    }
}

int main()
{
    int nx = 1024;
    int ny = 512;
    int ns = 100;
    
    unsigned int *pixels = new unsigned int[nx*ny];
    
    hitable* list[2];
    
    list[0] = new sphere(vec3(0, 0, -1), 0.5);
    list[1] = new sphere(vec3(0, -100.5, -1), 100);
    
    hitable* world = new hitable_list(list, 2);
    camera* cam = new camera();
        
    // Spawning threads
    bool kill = false;
    int threadCount = 1;
    threadCount = thread::hardware_concurrency(); // Enable Multithreading
    thread* threads = new thread[threadCount];
    for (int id=0; id<threadCount; id++)
        threads[id] = thread(worker, &kill, threadCount, id, nx, ny, ns, pixels, world, cam);
    
    // Wait until the window is closed
    show_window(nx, ny, pixels);
    
    // Terminate threads
    kill = true;
    for (int id=0; id<threadCount; id++)
        threads[id].join();
    
    return EXIT_SUCCESS;
}
