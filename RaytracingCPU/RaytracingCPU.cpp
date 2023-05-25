// RaytracingCPU.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#include <iostream>
#include "PPM.h"
#include "vec3.h"
#include "common.h"
#include "hittable.h"
#include "Camera.h"
#include "SphereHittable.h"
#include "DiffuseMaterial.h"
#include "MetalMaterial.h"
#include "DielectricMaterial.h"

static constexpr double aspectRatio3_2 = 3.0 / 2.0; 
static constexpr double aspectRatio16_9 = 16.0/9.0; 
static constexpr double aspectRatio = aspectRatio16_9;

static constexpr bool doGammaCorrection = true;

struct raytracing_state {
    //world
    HittableList world;
    
    //camera
    Camera camera;
} rt_state;



double clampNegPosToZeroPosOne(double v) {
    return (v + 1.0) * 0.5;
}

color NormalToColor(const vec3& n) {
    return color(clampNegPosToZeroPosOne(n[0]), clampNegPosToZeroPosOne(n[1]), clampNegPosToZeroPosOne(n[2]));
}

color CalcRayLighting(const Ray& r, const raytracing_state& rt_state, int depth) {
    if (depth <= 0)
        return color(0, 0, 0);

    vec3 unitDir = normalize(r.dir);
    //r.dir = unitDir;

    double k = (unitDir[1] + 1.0) * 0.5;
    color background = (1 - k) * color(1.0, 1.0, 1.0) + k * color(0.5, 0.7, 1.0);

    HitInfo hitInfo;
    if (rt_state.world.hit(r, 0.00001, c_infinity, hitInfo)) { //0.00001 is working for interesting eta's, I don't remember how did I come up with idea that it should be lesser to get rid of black regions on certain <eta> values

        color atten = color(0.0, 0.0, 0.0);
        Ray scattered;
        if (hitInfo.material->scatter(r, hitInfo, atten, scattered))
            return atten * CalcRayLighting(scattered, rt_state, depth - 1);
        return color(0.0, 0.0, 0.0);
    }

    return background;
}

void FlushToColorBuffer(color col, int& r, int& g, int& b) {
    col[0] = clamp(col[0], 0.0, 1.0);
    col[1] = clamp(col[1], 0.0, 1.0);
    col[2] = clamp(col[2], 0.0, 1.0);

    if (doGammaCorrection) {
        col[0] = sqrt(col[0]);
        col[1] = sqrt(col[1]);
        col[2] = sqrt(col[2]);
    }

    r = col[0] * 255.999;
    g = col[1] * 255.999;
    b = col[2] * 255.999;
}

void SetupScene1(raytracing_state& rt_state) {
    point3 origin = point3(3, 3, 2);
    point3 lookat = point3(0, 0, -1);
    double focusDist = (lookat - origin).length();//1.0;//(lookat - origin).length();
    double aperture = 0.5;
    rt_state.camera = Camera(origin, lookat, vec3(0, 1, 0), 20.0, 16.0 / 9.0, focusDist, aperture);

    auto material_ground = make_shared<DiffuseMaterial>(color(0.8, 0.8, 0.0));
    auto material_center = make_shared<DiffuseMaterial>(color(0.1, 0.2, 0.5));
    auto material_left = make_shared<DielectricMaterial>(1.5);
    auto material_right = make_shared<MetalMaterial>(color(0.8, 0.6, 0.2));

    rt_state.world = HittableList({ make_unique2<Hittable, SphereHittable>(point3(0.0, -100.5, -1.0), 100.0, material_ground),
        make_unique2<Hittable, SphereHittable>(point3(0.0, 0.0, -1.0), 0.5, material_center),
        make_unique2<Hittable, SphereHittable>(point3(-1.0, 0.0, -1.0), 0.5, material_left),
        make_unique2<Hittable, SphereHittable>(point3(-1.0, 0.0, -1.0), -0.45, material_left),
        make_unique2<Hittable, SphereHittable>(point3(1.0, 0.0, -1.0), 0.5, material_right) });
}

void SetupScene2(raytracing_state& rt_state) {
    point3 origin = point3(3, 3, 2);
    point3 lookat = point3(0, 0, -1);
    double focusDist = (lookat - origin).length();
    double aperture = 2.0;
    rt_state.camera = Camera(origin, lookat, vec3(0, 1, 0), 20.0, 16.0 / 9.0, focusDist, aperture);

    auto material_ground = make_shared<DiffuseMaterial>(color(0.8, 0.8, 0.0));
    auto material_center = make_shared<DiffuseMaterial>(color(0.1, 0.2, 0.5));
    auto material_left = make_shared<DielectricMaterial>(1.5);
    auto material_right = make_shared<MetalMaterial>(color(0.8, 0.6, 0.2));

    rt_state.world = HittableList({ make_unique2<Hittable, SphereHittable>(point3(0.0, -100.5, -1.0), 100.0, material_ground),
        make_unique2<Hittable, SphereHittable>(point3(0.0, 0.0, -1.0), 0.5, material_center),
        make_unique2<Hittable, SphereHittable>(point3(-1.0, 0.0, -1.0), 0.5, material_left),
        make_unique2<Hittable, SphereHittable>(point3(-1.0, 0.0, -1.0), -0.45, material_left),
        make_unique2<Hittable, SphereHittable>(point3(1.0, 0.0, -1.0), 0.5, material_right) });

    bool dbg = true;
}

void SetupSceneFinal1(raytracing_state& rt_state) {
    double focusDist = 10.0;
    double aperture = 0.1;
    point3 origin(13, 2, 3);
    point3 lookat(0, 0, 0);
    rt_state.camera = Camera(origin, lookat, vec3(0.0, 1.0, 0.0), 20, aspectRatio, focusDist, aperture);

    HittableList& world = rt_state.world;

    shared_ptr<Material> ground_material = make_shared<DiffuseMaterial>(color(0.5, 0.5, 0.5));
    world.add(make_unique<SphereHittable>(point3(0, -1000, 0), 1000, ground_material));

    int n = 8;

    for (int a = -n; a < n; a++) {
        for (int b = -n; b < n; b++) {
            auto choose_mat = random_double();
            point3 center(a + 0.9 * random_double(), 0.2, b + 0.9 * random_double());

            if ((center - point3(4, 0.2, 0)).length() > 0.9) {
                shared_ptr<Material> sphere_material;

                if (choose_mat < 0.6) {
                    // diffuse
                    auto albedo = color::random();
                    sphere_material = make_shared<DiffuseMaterial>(albedo);
                    world.add(make_unique<SphereHittable>(center, 0.2, sphere_material));
                }
                else if (choose_mat < 0.8) {
                    // metal
                    auto albedo = color::random(0.5, 1);

                    sphere_material = make_shared<MetalMaterial>(albedo);
                    world.add(make_unique<SphereHittable>(center, 0.2, sphere_material));
                }
                else if (choose_mat < 0.9) {
                    // glass
                    sphere_material = make_shared<DielectricMaterial>(1.5);
                    world.add(make_unique<SphereHittable>(center, 0.2, sphere_material));
                }
                else {
                    // air glass(wtf, but has an interesting look)
                    // the interesting look should be arising from the fact that the fresnel term is computed in approx terms
                    sphere_material = make_shared<DielectricMaterial>(1.0);
                    world.add(make_unique<SphereHittable>(center, 0.2, sphere_material));
                }
            }
        }
    }

    auto material1 = make_shared<DielectricMaterial>(1.5);
    world.add(make_unique<SphereHittable>(point3(0, 1, 0), 1.0, material1));

    auto material2 = make_shared<DiffuseMaterial>(color(0.4, 0.2, 0.1));
    world.add(make_unique<SphereHittable>(point3(-4, 1, 0), 1.0, material2));

    auto material3 = make_shared<MetalMaterial>(color(0.7, 0.6, 0.5));
    world.add(make_unique<SphereHittable>(point3(4, 1, 0), 1.0, material3));;
}

auto GetRaytracingSampleFunc(int width, int height) {
    static raytracing_state rt_state{};
    SetupScene2(rt_state);
    //SetupSceneFinal1(rt_state);

    //here comes actual raytracing code
    return [width, height](int x, int y, int& r, int& g, int& b) {
        static constexpr int samplesPerPixel = 100;
        static constexpr double samplesAveragingScale = 1.0 / double(samplesPerPixel);

        color rayLighting = vec3(0, 0, 0);

        for (int i = 0; i < samplesPerPixel; i++) {

            double u = (x + random_double()) / (width-1);
            double v = (y + random_double()) / (height-1);

             
            Ray ray = rt_state.camera.GetRay(u, v);

            rayLighting += CalcRayLighting(ray, rt_state, 50);
        }

        FlushToColorBuffer(rayLighting*samplesAveragingScale, r, g, b);
    };
}

int main()
{
    static constexpr int width = 400;//1200;
    static constexpr int height = static_cast<int>(width / aspectRatio);

    auto sampler = GetRaytracingSampleFunc(width, height);//GetSampleFunc(width, height, DemoSamplerPPMHelper);//DemoSamplerPPM<width, height>;
    WritePPM(width, height, sampler);
}

