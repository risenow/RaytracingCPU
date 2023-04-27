#pragma once
#include "hittable.h"

class SphereHittable : public Hittable {
public:
    SphereHittable(const point3& center, double r) : m_Center(center), m_R(r) {}

    virtual bool hit(const Ray& ray, double tMin, double tMax, HitInfo& hitInfo) const override {
        /*vec3 centerToOrigin = (ray.origin - m_Center);
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
        }*/

        vec3 oc = ray.origin - m_Center;
        auto a = ray.dir.length_2();
        auto half_b = dot(oc, ray.dir);
        auto c = oc.length_2() - m_R * m_R;

        auto discriminant = half_b * half_b - a * c;
        if (discriminant < 0) return false;
        auto sqrtd = sqrt(discriminant);

        // Find the nearest root that lies in the acceptable range.
        auto root = (-half_b - sqrtd) / a;
        if (root < tMin || tMax < root) {
            root = (-half_b + sqrtd) / a;
            if (root < tMin || tMax < root)
                return false;
        }

        hitInfo.t = root;
        hitInfo.pos = ray.at(hitInfo.t);
        hitInfo.n = (hitInfo.pos - m_Center) / m_R;

        return true;

        return true;
    }

private:
    point3 m_Center;
    double m_R;
};