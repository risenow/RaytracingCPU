#pragma once

#include "common.h"
#include "ray.h"
#include "hittable.h"

class Material{
public:
    virtual bool scatter(const Ray& incident, const HitInfo& hitInfo, color& attenuation, Ray& scattered) = 0;

};