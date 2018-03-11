//
//  main.c
//  pRat
//
//  Created by Sosoyan on 27/12/2017.
//  Copyright © 2017 Sosoyan. All rights reserved.
//
#include <math.h>
#include <stdlib.h>
#include <iostream>

#include "log.h"
#include "vec3.h"

using namespace std;

vec3 random_on_unit_sphere()
{
    vec3 p;
    do
    {
        p = 2.0*vec3(drand48(), drand48(), drand48()) - vec3(1,1,1);
    }
    while (dot(p,p) >= 1.0);
    return unit_vector(p);
}

inline float pdf(vec3& p)
{
    return 1 / (4 * M_PI);
}

int main()
{
    int n = 100;
    float r = 0;
    float sum = 0;

    for (int i=0; i<n; i++)
    {
        r = drand48();
        vec3 d = random_on_unit_sphere();
        float cosine_squared = d.z() * d.z();
        sum += cosine_squared / pdf(d);
        
        logRay(vec3(0,0,0), d);
    }
    
    //    int N = 100;
    //    float sum = 0;
    //
    //    for (int i=0; i<N; i++)
    //    {
    //        float x = 2*drand48();
    //        sum += x*x;
    //
    //        logVec(vec3(x, x*x, 0));
    //    }
    //
    //    cout << "I = " << 2 * sum / N << endl;
    
    
    //    // MonteCarlo Integration of x^2 over (0, 2)
    //    int n = 100000;
    //    int samples = 0;
    //    float sum = 0;
    //
    //    for (int i=0; i<n; i++)
    //    {
    //        // Standard MC Approch
    //        float x = 2*drand48(); // 0.0 to 2.0
    //        float y = 4*drand48(); // 0.0 to 4.0
    //        if (y <= x*x)
    //            samples++;
    //
    //        sum += x*x; // Summary of random samples on the curve
    //    }
    //
    //    // Standard MC Approach
    //    cout << float(samples) / float(n) * (2.0 * 4.0) << endl;
    //
    //    // Averaging the summary?
    //    cout << 2 * sum / n << endl;
    
    
    
    //    int inside_circle = 0;
    //    int inside_circle_stratified = 0;
    //    int sqrt_N = 1000;
    //
    //    for (int i=0; i<sqrt_N; i++)
    //    {
    //        for (int j=0; j<sqrt_N; j++)
    //        {
    //            float x = 2 * drand48() - 1; // -1.0 to 1.0
    //            float y = 2 * drand48() - 1; // -1.0 to 1.0
    //
    //            if (x*x + y*y < 1)
    //                inside_circle++;
    //
    //            x = 2 * ((float(i) + drand48()) / sqrt_N) - 1;
    //            y = 2 * ((float(j) + drand48()) / sqrt_N) - 1;
    //
    //
    //            if (x*x + y*y < 1)
    //                inside_circle_stratified++;
    //        }
    //    }
    //    std::cout << "Regular Estimage of Pi = " << 4 * float(inside_circle) / (sqrt_N*sqrt_N) << "\n";
    //    std::cout << "Stratified Estimage of Pi = " << 4 * float(inside_circle_stratified) / (sqrt_N*sqrt_N) << "\n";
    
    return 0;
}
