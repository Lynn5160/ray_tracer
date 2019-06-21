#include "window.h"
#include "vec3.h"
#include "ray.h"
#include "sphere.h"
#include "hitable.h"
#include "hitable_list.h"

using namespace std;

vec3 color(const ray& r, hitable* world)
{
    hit_record rec;
    if (world->hit(r, 0.0, MAXFLOAT, rec))
        return 0.5 * vec3(rec.normal.x() + 1, rec.normal.y() + 1, rec.normal.z() + 1);
    else
    {
        vec3 unit_direction = unit_vector(r.direction());
        float t = 0.5 * (unit_direction.y() + 1.0);
        return (1.0 - t) * vec3(1.0, 1.0, 1.0) + t * vec3(0.5, 0.7, 1.0);
    }
}

int main()
{
    int width = 1024;
    int height = 512;
    
    unsigned int *pixels = new unsigned int[width * height];
    
    vec3 lower_left_corner(-2.0, -1.0, -1.0);
    vec3 horizontal(4.0, 0.0, 0.0);
    vec3 vertical(0.0, 2.0, 0.0);
    vec3 origin(0.0, 0.0, 0.0);
    
    hitable* list[2];
    
    list[0] = new sphere(vec3(0, 0, -1), 0.5);
    list[1] = new sphere(vec3(0, -100.5, -1), 100);
    
    hitable* world = new hitable_list(list, 2);
    
    for (int j = height-1; j >= 0; j--)
    {
        for (int i=0; i < width; i++)
        {
            float u = float(i) / float(width);
            float v = float(j) / float(height);
            
            vec3 direction = unit_vector(lower_left_corner + (u * horizontal) + (v * vertical));

            ray r(origin, direction);
            vec3 col = color(r, world);

            // Converting to integers
            int ir = int(255.99 * col[0]);
            int ig = int(255.99 * col[1]);
            int ib = int(255.99 * col[2]);
            
            int idx = width * (height-j-1) + i;
            pixels[idx] = (ir << 16) + (ig << 8) + ib;
        }
    }
    
    // Wait until the window is closed
    show_window(width, height, pixels);
    
    return EXIT_SUCCESS;
}



