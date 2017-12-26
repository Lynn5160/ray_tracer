#include <iostream>
#include <fstream>
#include <thread>
#include <mutex>
#include <vector>

#include "log.h"
#include "sphere.h"
#include "moving_sphere.h"
#include "hitable_list.h"
#include "float.h"
#include "camera.h"
#include "material.h"
#include "bvh.h"
#include "aarect.h"

#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"

std::mutex locker;
void worker(int tc, int id, int nx, int ny, int ns, hitable* world, camera* cam, vec3* data);

vec3 color(const ray& r, hitable *world, int depth)
{
    hit_record rec;
    if (world->hit(r, 0.001, FLT_MAX, rec)) 
    {
        ray scattered;
        vec3 attenuation;
        vec3 emitted = rec.mat_ptr->emitted(rec.u, rec.v, rec.p);
        if (depth < 50 && rec.mat_ptr->scatter(r, rec, attenuation, scattered))
        {
            return emitted + attenuation*color(scattered, world, depth+1);
        }
        else
        {
            return emitted;
        }
    }
    else
    {
        vec3 unit_direction = unit_vector(r.direction());
        float t = 0.5*(unit_direction.y() + 1.0);
        return (1.0-t)*vec3(1.0, 1.0, 1.0) + t*vec3(0.5, 0.7, 1.0);
//        return vec3(0,0,0);
    }
}

hitable *random_scene()
{
    int n = 500;
    hitable **list = new hitable*[n+1];
    texture* texture = new checker_texture(new constant_texture(vec3(0.2, 0.3, 0.1)), new constant_texture(vec3(0.9, 0.9, 0.9)));
//    texture* texture = new noise_texture(1);
    list[0] =  new sphere(vec3(0,-1000,0), 1000, new lambertian(texture));
    int i = 1;
    for (int a = -11; a < 11; a++)
    {
        for (int b = -11; b < 11; b++)
        {
            float choose_mat = drand48();
            vec3 center(a+0.9*drand48(),0.2,b+0.9*drand48());
            if ((center-vec3(4,0.2,0)).length() > 0.9)
            {
                if (choose_mat < 0.8)
                {
                    // diffuse
                    list[i++] = new moving_sphere(center, center + vec3(0, 0.5 * drand48(), 0), 0.0, 1.0, 0.2, new lambertian(new constant_texture(vec3(drand48()*drand48(),
                                                                                                                                                        drand48()*drand48(),
                                                                                                                                                        drand48()*drand48()))));
                }
                else if (choose_mat < 0.95)
                {
                    // metal
                    list[i++] = new sphere(center, 0.2, new metal(vec3(0.5*(1 + drand48()), 0.5*(1 + drand48()), 0.5*(1 + drand48())),  0.5*drand48()));
                }
                else
                {
                    // glass
                    list[i++] = new sphere(center, 0.2, new dielectric(1.5));
                }
            }
        }
    }
    
    list[i++] = new sphere(vec3(0, 1, 0), 1.0, new dielectric(1.5));
    int nx, ny, nn;
    unsigned char *tex_data = stbi_load("/Volumes/Work-HD/Development/pRat_project/pRat/img/earthmap.jpg", &nx, &ny, &nn, 0);
    list[i++] = new sphere(vec3(-4, 1, 0), 1.0, new lambertian(new image_texture(tex_data, nx, ny)));
    list[i++] = new sphere(vec3(4, 1, 0), 1.0, new metal(vec3(0.7, 0.6, 0.5), 0.0));
    
    return new bvh_node(list,i, 0.0, 1.0);
}

hitable* simple_scene()
{
        hitable *list[4];
        texture* checker = new checker_texture(new constant_texture(vec3(0.2, 0.3, 0.1)), new constant_texture(vec3(0.9, 0.9, 0.9)));
        list[0] = new sphere(vec3(0,-100.5,-1), 100, new lambertian(checker));
        list[1] = new moving_sphere(vec3(0,0,-1), vec3(0,0,-1) + vec3(0, 0.25, 0.25), 0.0, 1.0, 0.5, new lambertian(new constant_texture(vec3(0.1, 0.2, 0.5))));
        list[2] = new sphere(vec3(1,0,-1), 0.5, new metal(vec3(0.8, 0.6, 0.2), 0.0));
        list[3] = new sphere(vec3(-1,0,-1), 0.5, new dielectric(1.5));
    
    return  new bvh_node(list, 4, 0.0, 1.0);
}

hitable* simple_light()
{
    texture *pertext = new noise_texture(4);
    hitable **list = new hitable*[4];
    list[0] =  new sphere(vec3(0,-1000, 0), 1000, new lambertian( pertext ));
    list[1] =  new sphere(vec3(0, 2, 0), 2, new lambertian( pertext ));
    list[2] =  new sphere(vec3(0, 7, 0), 2, new diffuse_light( new constant_texture(vec3(4,4,4))));
    list[3] =  new xy_rect(3, 5, 1, 3, -2, new diffuse_light(new constant_texture(vec3(4,4,4))));
    return new hitable_list(list,4);
}

int main()
{
//    Simple Scene
//    hitable* world = simple_scene();
//    hitable *world = simple_light();
//    vec3 lookfrom(13,2,3);
//    vec3 lookat(0, 0, -1);
    
//    Random Scene
    hitable *world = random_scene();
    vec3 lookfrom(13,2,3);
    vec3 lookat(0,0,0);

    int nx = 512;
    int ny = 256;
    int ns = 100;
    float dist_to_focus = 10.0;
    float aperture = 0.1;
    float vfov = 20.0;
    
    camera* cam = new camera(lookfrom, lookat, vec3(0,1,0), vfov, float(nx)/float(ny), aperture, dist_to_focus, 0.0, 1.0);
    
    // Spawning threads
    vec3* data = new vec3[nx*ny];
    int tc = std::thread::hardware_concurrency();
    std::thread* threads = new std::thread[tc];
    for (int i=1; i<=tc; i++) threads[i-1] = std::thread(worker, tc, i, nx, ny, ns, world, cam, data);
    for (int i=1; i<=tc; i++) threads[i-1].join();
    
    // Image writing
    std::ofstream image;
    image.open ("image.ppm");
    image << "P3\n" << nx << " " << ny << "\n255\n";
    for (int i=0; i<nx*ny; i++)
        image << data[i] << "\n";
	image.close();
	return 0;
}

void worker(int tc, int id, int nx, int ny, int ns, hitable* world, camera* cam, vec3* data)
{
    int ny1 = ny / tc * id;
    int ny2 = ny1 - (ny / tc);
    
    for (int j = ny1-1; j >= ny2; j--)
    {
        for (int i=0; i<nx; i++)
        {
            vec3 col(0, 0, 0);
            
            for (int s=0; s < ns; s++)
            {
                float u = float(i + drand48()) / float(nx);
                float v = float(j + drand48()) / float(ny);
                
                ray r = cam->get_ray(u, v);
                col += color(r, world, 0);
            }
            
            col /= float(ns);
            col = vec3(sqrt(col[0]), sqrt(col[1]), sqrt(col[2]));
            int ir = int(255.99*col[0]);
            int ig = int(255.99*col[1]);
            int ib = int(255.99*col[2]);
            
            locker.lock();
            data[nx*(ny-j-1)+i] = vec3(ir, ig, ib);
            locker.unlock();
        }
    }
}


