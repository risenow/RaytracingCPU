#pragma once

#include <numeric>
#include <cmath>
#include <memory>
#include <random>


using std::make_shared;
using std::shared_ptr;

constexpr double c_infinity = std::numeric_limits<double>::infinity();
constexpr double c_pi = 3.1415926535897932385;

inline double clamp(double v, double min, double max) {
    return std::min(std::max(v, min), max);
}

inline double degrees2rads(double degrees) {
    return degrees * c_pi / 180.0;
}

/*inline double random_double(double min, double max) {
    static std::uniform_real_distribution<double> dist(min, max);
    static std::mt19937 engine;

    return dist(engine);
}
inline double random_double() {
    return random_double(0.0, 1.0);
}*/

inline double random_double() {
    // Returns a random real in [0,1).
    return rand() / (RAND_MAX + 1.0);
}

inline double random_double(double min, double max) {
    // Returns a random real in [min,max).
    return min + (max - min) * random_double();
}