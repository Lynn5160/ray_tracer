#include "window.h"
#include "vec3.h"


int main()
{
    int width = 512;
    int height = 512;
    
    unsigned int *pixels = new unsigned int[width * height];
    
    for (int j = height-1; j >= 0; j--)
    {
        for (int i=0; i < width; i++)
        {
            vec3 col(float(i) / float(width), float(j) / float(height), 0.0);
            
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



