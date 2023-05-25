#pragma once
#include "vec3.h"
#include "ray.h"
#include <numeric>

class AABB {
private:
    static constexpr double _doubleMin = -std::numeric_limits<double>::max();
    static constexpr double _doubleMax = std::numeric_limits<double>::max();
public:
    AABB() : m_Min(vec3(_doubleMax, _doubleMax, _doubleMax)), m_Max(vec3(_doubleMin, _doubleMin, _doubleMin)) {}
    AABB(vec3 min, vec3 extent) : m_Min(min), m_Max(m_Min + extent) {}

    double area() const {
        vec3 ext = extent();
        return ext[0] * ext[1] * 2 + ext[0] * ext[2] * 2 + ext[1] * ext[2] * 2;
    }

    vec3 center() const {
        return (m_Min + m_Max) * 0.5;
    }

    vec3 extent() const {
        return m_Max - m_Min;
    }

    void extend(const AABB& other) {
        m_Min = vec3(std::min(m_Min[0], other.m_Min[0]), std::min(m_Min[1], other.m_Min[1]), std::min(m_Min[2], other.m_Min[2]));
        m_Max = vec3(std::max(m_Max[0], other.m_Max[0]), std::max(m_Max[1], other.m_Max[1]), std::max(m_Max[2], other.m_Max[2]));
    }

    void extend(const point3& p) {
        AABB auxBB = AABB(p, vec3(0, 0, 0));
        extend(auxBB);
    }


    bool intersect(const Ray& r, double tmin, double tmax) {
        for (size_t e = 0; e < 3; e++) {
            double invD = 1.0 / r.dir[e];
            double te_t0 = (m_Min[e] - r.origin[e]) * invD;
            double te_t1 = (m_Max[e] - r.origin[e]) * invD;
            if (invD < 0.0)
                std::swap(te_t0, te_t1);

            tmin = std::max(tmin, te_t0);
            tmax = std::min(tmax, te_t1);

            if (tmin >= tmax)
                return false;
        }

        return true;
    }

    vec3 m_Min;
    vec3 m_Max;
};