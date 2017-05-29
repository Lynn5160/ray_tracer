#ifndef MATERIALH
#define MATERIALH 

struct hit_record;

#include "ray.h"
#include "hitable.h"

void logRays(const ray& r);
void logRays(const vec3& o, const vec3& d);

float schlick(float cosine, float ref_idx)
{
    float r0 = (1-ref_idx) / (1+ref_idx);
    r0 = r0*r0;
    return r0 + (1-r0)*pow((1 - cosine),5);
}

bool refract(const vec3& v, const vec3& n, float ni_over_nt, vec3& refracted)
{
    vec3 uv = unit_vector(v);
    float dt = dot(uv, n);
    
    float sinTheta = sqrt(1+dt) / ni_over_nt;
    
    float discriminant = 1.0 - ni_over_nt*ni_over_nt*(1-dt*dt);
    if (discriminant > 0)
    {
        refracted = ni_over_nt*(uv - n*dt) - n*sqrt(discriminant);
        float cos = dot(v, refracted);
        float cosTheta = sqrt(1 - sinTheta*sinTheta);
        logVar(cosTheta);

        return true;
    }
    else 
        return false;
}


vec3 reflect(const vec3& v, const vec3& n)
{
    vec3 result =  v - 2*dot(v,n)*n;
    return result;
}


vec3 random_in_unit_sphere()
{
    vec3 p;
    do
    {
        p = 2.0*vec3(drand48(),drand48(),drand48()) - vec3(1,1,1);
    }
    while (p.squared_length() >= 1.0);
    return p;
}


class material
{
    public:
        virtual bool scatter(const ray& r_in, const hit_record& rec, vec3& attenuation, ray& scattered) const = 0;
};

class lambertian : public material
{
    public:
        lambertian(const vec3& a) : albedo(a) {}
        virtual bool scatter(const ray& r_in, const hit_record& rec, vec3& attenuation, ray& scattered) const
        {
             vec3 target = rec.p + rec.normal + random_in_unit_sphere();
             scattered = ray(rec.p, target-rec.p);
             attenuation = albedo;
             return true;
        }

        vec3 albedo;
};

class metal : public material
{
    public:
        metal(const vec3& a, float f) : albedo(a)
        {
            if (f < 1) fuzz = f; else fuzz = 1;
        }
    
        virtual bool scatter(const ray& r_in,
                             const hit_record& rec,
                             vec3& attenuation,
                             ray& scattered) const
        {
            vec3 reflected = reflect(r_in.direction(), rec.normal);
            scattered = ray(rec.p, reflected + fuzz*random_in_unit_sphere());
            attenuation = albedo;
            return (dot(scattered.direction(), rec.normal) > 0);
        }
        vec3 albedo;
        float fuzz;
};

class dielectric : public material
{
    public:
        dielectric(float ri) : ref_idx(ri) {}
    
        virtual bool scatter(const ray& r_in,
                             const hit_record& rec,
                             vec3& attenuation,
                             ray& scattered) const
        {
            attenuation = vec3(1.0, 1.0, 1.0);

            vec3 outward_normal;
            vec3 reflected = reflect(r_in.direction(), rec.normal);
            vec3 refracted;

            float ni_over_nt;
            float reflected_prob;
            float cosine = 0.0;
            
            float dotRN = dot(r_in.direction(), rec.normal);
            
            if (dotRN > 0)
            {
                outward_normal = -rec.normal;
                ni_over_nt = ref_idx;
                cosine = ref_idx * dotRN / r_in.direction().length();
                cosine = sqrt(1 - ref_idx*ref_idx*(1-cosine*cosine));
            }
            else
            {
                outward_normal = rec.normal;
                ni_over_nt = 1.0 / ref_idx;
                cosine = -dotRN / r_in.direction().length();
            }

            if (refract(r_in.direction(), outward_normal, ni_over_nt, refracted))
                reflected_prob = schlick(cosine, ref_idx);
            else
                reflected_prob = 1.0;
            
            if (drand48() < reflected_prob)
            {
                scattered = ray(rec.p, reflected);
            }
            else
            {
                scattered = ray(rec.p, refracted);
            }
            
            return true;
        }

        float ref_idx;
};

#endif



