#pragma once
#include <cmath>
#include "common.h"

class vec3;
vec3 normalize(const vec3& u);

class vec3 {
public:
    double e[3];

    vec3() : e{0.0, 0.0, 0.0} {}
    vec3(double x, double y, double z) : e{x,y,z} {}

    double operator [](size_t i) const { return e[i]; }
    double& operator [](size_t i) { return e[i]; }

    vec3 operator -() const {
        return vec3(-e[0], -e[1], -e[2]);
    }

    vec3& operator +=(const vec3& other) {
        e[0] += other.e[0];
        e[1] += other.e[1];
        e[2] += other.e[2];
        return *this;
    }

    vec3& operator *=(double t) {
        e[0] *= t;
        e[1] *= t;
        e[2] *= t;
        return *this;
    }
    vec3& operator /=(double t) {
        return *this *= (1 / t);
    }

    double length_2() const {
        return e[0] * e[0] + e[1] * e[1] + e[2] * e[2];
    }
    double length() const {
        return sqrt(length_2());
    }

    static vec3 random() {
        return vec3(random_double(), random_double(), random_double());
    }
    static vec3 random(double min, double max) {
        return vec3(random_double(min, max), random_double(min, max), random_double(min, max));
    }

    static vec3 random_in_unit_sphere() {
        vec3 v;
        while (true)
        {
            v = vec3::random(-1.0, 1.0);

            if (v.length_2() <= 1)
                return v;
        }

        return v;
    }
    static vec3 random_unit_vec() {
        return normalize(random_in_unit_sphere());
    }

    static vec3 cos3_distr_vec(const vec3& n);
    static vec3 lambert_distr_vec(const vec3& n);
    //using rejection method
    static vec3 uniform_hemisphere_distr_rej(const vec3& n);
};

using point3 = vec3;
using color = vec3;

inline vec3 operator +(const vec3& u, const vec3& v) {
    return vec3(u[0] + v[0], u[1] + v[1], u[2] + v[2]);
}

inline vec3 operator-(const vec3& u, const vec3& v) {
    return vec3(u[0] - v[0], u[1] - v[1], u[2] - v[2]);
}

inline vec3 operator* (const vec3& u, const vec3& v) {
    return vec3(u[0] * v[0], u[1] * v[1], u[2] * v[2]);
}
inline vec3 operator* (double t, const vec3& u) {
    return u * vec3(t, t, t);
}
inline vec3 operator*(const vec3& u, double t) {
    return t * u;
}
inline vec3 operator/(const vec3& u, double t) {
    return (1 / t) * u;
}

inline double dot(const vec3& u, const vec3& v) {
    return u[0] * v[0] + u[1] * v[1] + u[2] * v[2];
}

inline vec3 cross(const vec3& u, const vec3& v) {
    return vec3(u.e[1] * v.e[2] - u.e[2] * v.e[1],
        u.e[2] * v.e[0] - u.e[0] * v.e[2],
        u.e[0] * v.e[1] - u.e[1] * v.e[0]);
}