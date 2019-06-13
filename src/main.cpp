#include "window.h"


int main()
{
    int width = 512;
    int height = 512;
    
    unsigned int *pixels = new unsigned int[width*height];
    
    for (int j = height-1; j >= 0; j--)
    {
        for (int i=0; i < width; i++)
        {
            float r = float(i) / float(width);
            float g = float(j) / float(width);
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



