#pragma once
#include <vector>
#include <memory>
#include "ray.h"
#include "common.h"
#include "AABB.h"

class Material;

struct HitInfo {
    HitInfo(double _t = 0.0) : t(_t), frontFace(false), objId(-1) {}

    point3 pos;
    vec3 n;
    double t;
    bool frontFace;
    std::shared_ptr<Material> material;

    void setFaceNormal(const Ray& ray, const vec3& outN) {
        if (dot(ray.dir, outN) < 0)
        {
            frontFace = true;
            n = outN;
        }
        else {
            frontFace = false;
            n = -outN;
        }
    }

    int64_t objId;
};

class Hittable {
public:
    virtual bool hit(const Ray& ray, double tMin, double tMax, HitInfo& hitInfo) const = 0;
    virtual bool boundingBox(AABB& aabb) const = 0;
};

class HittableList : public Hittable {
public:
    HittableList() {}
    HittableList(std::initializer_list<il_unique_ptr_wrapper<Hittable>> objs) : m_Objects(std::make_move_iterator(objs.begin()), std::make_move_iterator(objs.end())) {}
    HittableList(HittableList&& other) {
        m_Objects = std::move(other.m_Objects);
    }

    void operator =(HittableList&& other){
        m_Objects = std::move(other.m_Objects);
    }

    HittableList(const HittableList&) = delete;
    void operator =(const HittableList&) = delete;


    void clear() {
        m_Objects.clear();
    }
    virtual void add(unique_ptr<Hittable>&& obj) {
        m_Objects.push_back(std::move(obj));
    }

    template<typename... Args>
    void add(unique_ptr<Hittable>&& obj, Args... args) {
        add(std::move(obj));
        add(args...);
    }

    virtual bool boundingBox(AABB& bb) const override {
        bb = AABB();

        for (auto& obj : m_Objects) {
            AABB objBB;
            if (obj->boundingBox(objBB))
                bb.extend(objBB);
        }

        return true;
    }

    virtual bool hit(const Ray& ray, double tMin, double tMax, HitInfo& hitInfo) const override{
        bool hitAnything = false;

        hitInfo.t = tMax;

        HitInfo hitInfoTemp{};
        for (size_t i = 0; i < m_Objects.size(); i++) {
            const unique_ptr<Hittable>& hittable = m_Objects[i];
            if (hittable->hit(ray, tMin, tMax, hitInfoTemp) && hitInfoTemp.t < hitInfo.t) {
                hitInfoTemp.objId = i;

                hitInfo = hitInfoTemp;
                hitAnything = true;
            }
        }

        return hitAnything;
    }
protected:
    std::vector<unique_ptr<Hittable>> m_Objects;
};