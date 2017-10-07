#ifndef LOGH
#define LOGH

#include "ray.h"

void logRay(const ray& r)
{
    using namespace std;
    cout << r.origin().x() << "," << r.origin().y() << "," << r.origin().z() << ",";
    cout << r.direction().x() << "," << r.direction().y() << "," << r.direction().z() << ",";
}

void logRay(const vec3& o, const vec3& d)
{
    using namespace std;
    cout << o.x() << "," << o.y() << "," << o.z() << ",";
    cout << d.x() << "," << d.y() << "," << d.z() << ",";
}

void logVec(const vec3& v)
{
    using namespace std;
    cout << v.x() << "," << v.y() << "," << v.z() << ",";
}

template<typename T>
void logVar(const T& var)
{
    using namespace std;
    std::cout << var;
}

#endif
