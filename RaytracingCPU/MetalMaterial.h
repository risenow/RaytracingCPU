#pragma once
#include "Material.h"

class MetalMaterial : public Material {
public:
    MetalMaterial() {}
    MetalMaterial(const color& albedo) : m_Albedo(albedo) {}

    virtual bool scatter(const Ray& incident, const HitInfo& hitInfo, color& atten, Ray& scattered) {
        vec3 dir = reflect(normalize(incident.dir), hitInfo.n);
        scattered = Ray(hitInfo.pos, normalize(dir));

        atten = m_Albedo;

        return dot(dir, hitInfo.n) > 0.0;
    }
private:
    color m_Albedo;
};