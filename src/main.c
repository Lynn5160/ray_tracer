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

inline float pdf(float x)
{
    return x * 0.5;
}

int main()
{
    int n = 10000;
    float r = 0;
    float sum = 0;
    
    for (int i=0; i<n; i++)
    {
        r += 3 * (1.0 / n);
//        r = drand48();
        float x = sqrt(4 * r);
        float p = pdf(x);
        sum += r*r;

//        logVec(vec3(sum, 0, 0));
    }

    cout << "I = " << sum / n << endl;
    
    
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
