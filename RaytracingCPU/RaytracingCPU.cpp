// RaytracingCPU.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#include <iostream>
#include "PPM.h"
#include "vec3.h"
#include "common.h"
#include "hittable.h"
#include "Camera.h"
#include "SphereHittable.h"

static constexpr double aspectRatio = 16.0 / 9.0;
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

    double k = (unitDir[1] + 1.0) * 0.5;
    color background = (1 - k) * color(1.0, 1.0, 1.0) + k * color(0.5, 0.7, 1.0);

    HitInfo hitInfo;
    if (rt_state.world.hit(r, 0.0001, c_infinity, hitInfo)) {
        //vec3 ruv = vec3::random_in_unit_sphere();
        //double dbgLen = ruv.length();
        //double dbgLen2 = ruv.length_2();
        //vec3 target = hitInfo.pos + hitInfo.n + ruv;
        vec3 dir = vec3::uniform_hemisphere_distr_rej(hitInfo.n);//vec3::lambert_distr_vec(hitInfo.n);

        Ray childRay = Ray(hitInfo.pos, dir);
        return 0.5 * CalcRayLighting(childRay, rt_state, depth - 1);
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

auto GetRaytracingSampleFunc(int width, int height) {
    raytracing_state rt_state{};

    rt_state.world = HittableList({
        make_shared<SphereHittable>(point3(0.0, -100.5, -1.0), 100.0),
        make_shared<SphereHittable>(point3(0.0, 0.0, -1.0), 0.5)
        });
    /* variadic
    rt_state.world.add(
        make_shared<SphereHittable>(point3(0.0, 0.0, -1.0), 0.5),
        make_shared<SphereHittable>(point3(0.0, -100.5, -1.0), 100.0));*/

    //here comes actual raytracing code
    return [rt_state, width, height](int x, int y, int& r, int& g, int& b) {
        static constexpr int samplesPerPixel = 100;
        static constexpr double samplesAveragingScale = 1.0 / double(samplesPerPixel);

        color rayLighting = vec3(0, 0, 0);

        for (int i = 0; i < samplesPerPixel; i++) {
            //double u = (double(x) + random_double()) / width;
            //double v = (double(y) + random_double()) / height;

            double u = (double(x) + random_double()) / (width-1);
            double v = (double(y) + random_double()) / (height-1);


            Ray ray = rt_state.camera.GetRay(u, v);

            rayLighting += CalcRayLighting(ray, rt_state, 50);
        }

        FlushToColorBuffer(rayLighting*samplesAveragingScale, r, g, b);
    };
}

int main()
{
    static constexpr int width = 400;
    
    static constexpr int height = static_cast<int>(width / aspectRatio);



    auto sampler = GetRaytracingSampleFunc(width, height);//GetSampleFunc(width, height, DemoSamplerPPMHelper);//DemoSamplerPPM<width, height>;
    WritePPM(width, height, sampler);
}

// Run program: Ctrl + F5 or Debug > Start Without Debugging menu
// Debug program: F5 or Debug > Start Debugging menu

// Tips for Getting Started: 
//   1. Use the Solution Explorer window to add/manage files
//   2. Use the Team Explorer window to connect to source control
//   3. Use the Output window to see build output and other messages
//   4. Use the Error List window to view errors
//   5. Go to Project > Add New Item to create new code files, or Project > Add Existing Item to add existing code files to the project
//   6. In the future, to open this project again, go to File > Open > Project and select the .sln file
