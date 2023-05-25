#pragma once

#include <numeric>
#include <cmath>
#include <memory>
#include <random>


using std::make_shared;
using std::shared_ptr;
using std::make_unique;
using std::unique_ptr;

template<class T>
//inititalizer list movable
struct il_movable{
    mutable T v;

    operator T() const&& {
        return std::move(v);
    }
    il_movable(T&& other) : v(std::move(other)) {}
};


template<class T>
using il_unique_ptr_wrapper = il_movable<unique_ptr<T>>;


//_Ty is class of unique_ptr
//_Ty2 is actual underlying object type(derrived class of _Ty) which is being construected
template <class _Ty, class _Ty2, class... _Types>
unique_ptr<_Ty> make_unique2(_Types&&... _Args) { // make a unique_ptr of base type while constructing derrived
    return unique_ptr<_Ty>(new _Ty2(_STD forward<_Types>(_Args)...));
}

constexpr double c_infinity = std::numeric_limits<double>::infinity();
constexpr double c_pi = 3.1415926535897932385;

inline double clamp(double v, double min, double max) {
    return std::min(std::max(v, min), max);
}

inline double degrees2rads(double degrees) {
    return degrees * (c_pi / 180.0);
}


inline double random_double() {
    static std::mt19937 engine;
    static std::uniform_real_distribution<double> dist(0.0, 1.0);

    return dist(engine);
}
inline double random_double(double min, double max) {
    return min + (max - min) * random_double();
}
