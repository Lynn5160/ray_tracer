#ifndef LOGH
#define LOGH

#include "ray.h"

using namespace std;

void logRay(const ray& r)
{
    vec3 ray_tip = r.origin() + r.direction();
    cout << r.origin().x() << "," << r.origin().y() << "," << r.origin().z() << ",";
    cout << ray_tip.x() << "," << ray_tip.y() << "," << ray_tip.z() << ",";
}

void logRay(const vec3& o, const vec3& d)
{
    cout << o.x() << "," << o.y() << "," << o.z() << ",";
    cout << (o + d).x() << "," << (o + d).y() << "," << (o + d).z() << ",";
}

void logVec(const vec3& v)
{
    cout << v.x() << "," << v.y() << "," << v.z() << ",";
}

template<typename T>
void logVar(const T& var)
{
    std::cout << var;
}

#endif
