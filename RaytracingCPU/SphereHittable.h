#pragma once
#include "hittable.h"
#include "Material.h"

class SphereHittable : public Hittable {
public:
    SphereHittable(const point3& center, double r, shared_ptr<Material> material) : m_Center(center), m_R(r), m_Material(material) {}

    virtual bool boundingBox(AABB& aabb) const override {
        vec3 halfExtent = vec3(m_R, m_R, m_R);
        aabb = AABB(m_Center - halfExtent, 2 * halfExtent);

        return true;
    }

    virtual bool hit(const Ray& ray, double tMin, double tMax, HitInfo& hitInfo) const override {
        vec3 centerToOrigin = (ray.origin - m_Center);
        double centerToOrigin2 = centerToOrigin.length_2();
        double dir2 = ray.dir.length_2();

        double a = dir2;
        double halfB = dot(centerToOrigin, ray.dir); //half b
        double c = centerToOrigin2 - m_R * m_R;

        double d = halfB * halfB - a * c;

        if (d < 0)
            return false;

        double t = (-halfB - sqrt(d)) / (a);
        if (t < tMin || t > tMax) {
            t = (-halfB + sqrt(d)) / a;
            if (t < tMin || t > tMax)
                return false;
        }

        hitInfo.t = t;
        hitInfo.pos = ray.at(t);
        hitInfo.n = (hitInfo.pos - m_Center)/m_R;
        hitInfo.frontFace = true;
        if (dot(hitInfo.n, ray.dir) > 0.0) {
            hitInfo.n = -hitInfo.n;
            hitInfo.frontFace = false;
        }
        hitInfo.material = m_Material;

       
        return true;
    }

private:
    shared_ptr<Material> m_Material;
    point3 m_Center;
    double m_R;
};