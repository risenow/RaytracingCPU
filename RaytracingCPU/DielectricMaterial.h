#pragma once

#include "Material.h"

class DielectricMaterial : public Material {
public:
    DielectricMaterial(double refrIndex) : m_RefractionIndex(refrIndex) {}

    virtual bool scatter(const Ray& incident, const HitInfo& hitInfo, color& atten, Ray& scattered) {
        double ri = hitInfo.frontFace ? 1.0 / m_RefractionIndex : m_RefractionIndex;

        atten = color(1, 1, 1);

        vec3 scatteredDir;

        vec3 unitIncidentDir = normalize(incident.dir);
        double cosine = -dot(unitIncidentDir, hitInfo.n);

        if (!refract(unitIncidentDir, hitInfo.n, scatteredDir, ri) || random_double() < reflectance(cosine, ri) )
            scatteredDir = reflect(incident.dir, hitInfo.n);
        
        scattered = Ray(hitInfo.pos, scatteredDir);

        return true;
    }
private:
    //schlick
    double reflectance(double cosine, double eta) {
        double r0 = (1 - eta) / (1 + eta);
        r0 = r0 * r0;
        return r0 + (1 - r0) * pow((1 - cosine), 5.0);
    }

    double m_RefractionIndex;
};