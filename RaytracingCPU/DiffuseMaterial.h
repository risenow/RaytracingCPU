#pragma once
#include "Material.h"

class DiffuseMaterial : public Material {
public:
    DiffuseMaterial() {}
    DiffuseMaterial(const color& albedo) : m_Albedo(albedo) {}

    virtual bool scatter(const Ray& incident, const HitInfo& hitInfo, color& albedo, Ray& scattered) override {
        vec3 scatteredDir = vec3::lambert_distr_vec(hitInfo.n);//vec3::lambert_distr_vec(hitInfo.n);
        if (scatteredDir.nearZero())
            scatteredDir = vec3::lambert_distr_vec(hitInfo.n);//hitInfo.n;

        scattered = Ray(hitInfo.pos, (scatteredDir));

        albedo = m_Albedo;

        return true;
    }
private:
    color m_Albedo;
};