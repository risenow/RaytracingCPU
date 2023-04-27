#pragma once
#include "vec3.h"

class Ray {
public:
    vec3 dir;
    vec3 origin;

    Ray(const point3& _orig, const vec3& _dir) : origin(_orig), dir(_dir) {}

    point3 at(double t) const {
        return origin + dir * t;
    }


};