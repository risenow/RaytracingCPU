#pragma once
#include <vector>
#include <memory>
#include "ray.h"
#include "common.h"


struct HitInfo {
    point3 pos;
    vec3 n;
    double t;
    bool frontFace;
    int objId;
};

class Hittable {
public:
    virtual bool hit(const Ray& ray, double tMin, double tMax, HitInfo& hitInfo) const = 0;
};

class HittableList : public Hittable {
public:
    HittableList() {}
    HittableList(std::initializer_list<shared_ptr<Hittable>> objs) : m_Objects(objs) {}
    void clear() {
        m_Objects.clear();
    }
    void add(shared_ptr<Hittable> obj) {
        m_Objects.push_back(obj);
    }
    template<typename... Args>
    void add(shared_ptr<Hittable> obj, Args... args) {
        add(obj);
        add(args...);
    }

    virtual bool hit(const Ray& ray, double tMin, double tMax, HitInfo& hitInfo) const override{
        /*bool hitAnything = false;

        hitInfo.t = tMax;

        HitInfo hitInfoTemp{};
        for (size_t i = 0; i < m_Objects.size(); i++) {
            shared_ptr<Hittable> hittable = m_Objects[i];
            if (hittable->hit(ray, tMin, tMax, hitInfoTemp) && hitInfoTemp.t < hitInfo.t) {
                hitInfoTemp.objId = i;

                hitInfo = hitInfoTemp;
                hitAnything = true;
            }
        }

        return hitAnything;*/
        HitInfo temp_rec;
        bool hit_anything = false;
        auto closest_so_far = tMax;

        for (const auto& object : m_Objects) {
            if (object->hit(ray, tMin, closest_so_far, temp_rec)) {
                hit_anything = true;
                closest_so_far = temp_rec.t;
                hitInfo = temp_rec;
            }
        }

        return hit_anything;
    }
private:
    std::vector<shared_ptr<Hittable>> m_Objects;
};