#ifndef LOGH
#define LOGH

#include "ray.h"

void logRay(const ray& r)
{
    using namespace std;
    vec3 ray_tip = r.origin() + r.direction();
    cout << r.origin().x() << "," << r.origin().y() << "," << r.origin().z() << ",";
    cout << ray_tip.x() << "," << ray_tip.y() << "," << ray_tip.z() << ",";
}

void logRay(const vec3& o, const vec3& d)
{
    using namespace std;
    cout << o.x() << "," << o.y() << "," << o.z() << ",";
    cout << (o + d).x() << "," << (o + d).y() << "," << (o + d).z() << ",";
}

void logVec(const vec3& v)
{
    using namespace std;
    cout << v.x() << "," << v.y() << "," << v.z() << ",";
}

template<typename T>
void logVar(const T& var, bool comma=true)
{
    using namespace std;
    if (comma)
        cout << var << ",";
    else
        cout << var << endl;
}

template<typename T>
void logProgress(const T& var)
{
    using namespace std;
    cout << "Progress: " << var << "%" << endl;
}

#endif
