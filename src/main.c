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

    return 0;
}
