#include <thread>
#include <SDL2/SDL.h>

#include "window.h"
#include "vec3.h"


int main()
{
    int nx = 512;
    int ny = 512;
    
    unsigned int *pixels = new unsigned int[nx*ny];
    
    for (int j = ny-1; j >= 0; j--)
    {
        for (int i=0; i < nx; i++)
        {
            vec3 col(float(i) / float(nx), float(j) / float(ny), 0.0);
            
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



