#include "window.h"
#include "vec3.h"
#include "ray.h"

using namespace std;

vec3 color(const ray& r)
{
    float t = 0.5 * (r.direction().y() + 1.0);
    return (1.0 - t) * vec3(1.0, 1.0, 1.0) + t * vec3(0.5, 0.7, 1.0);
}

int main()
{
    int nx = 512;
    int ny = 512;
    
    unsigned int *pixels = new unsigned int[nx*ny];
    
    vec3 lower_left_corner(-2.0, -1.0, -1.0);
    vec3 horizontal(4.0, 0.0, 0.0);
    vec3 vertical(0.0, 2.0, 0.0);
    vec3 origin(0.0, 0.0, 0.0);
    
    for (int j = ny-1; j >= 0; j--)
    {
        for (int i=0; i < nx; i++)
        {
            float u = float(i) / float(nx);
            float v = float(j) / float(ny);
            
            vec3 direction = unit_vector(lower_left_corner + (u * horizontal) + (v * vertical));

            ray r(origin, direction);
            vec3 col = color(r);
            
            // Converting to integers
            int ir = int(255.99 * col[0]);
            int ig = int(255.99 * col[1]);
            int ib = int(255.99 * col[2]);
            
            int idx = nx * (ny-j-1) + i;
            pixels[idx] = (ir << 16) + (ig << 8) + ib;
        }
    }
    
    // Wait until the window is closed
    show_window(nx, ny, pixels);
    
    return EXIT_SUCCESS;
}



