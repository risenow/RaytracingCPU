#include "vec3.h"

vec3 normalize(const vec3& u) {
    return u / u.length();
}

vec3 vec3::cos3_distr_vec(const vec3& n) {
    return n + random_in_unit_sphere();
}
vec3 vec3::lambert_distr_vec(const vec3& n) {
    return random_unit_vec() + n;
}
//using rejection method
vec3 vec3::uniform_hemisphere_distr_rej(const vec3& n) {
    vec3 v = vec3::random_unit_vec();

    if (dot(v, n) > 0.0) {
        return v;
    }
    return -v;
}