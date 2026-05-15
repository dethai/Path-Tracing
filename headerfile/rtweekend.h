#ifndef RTWEEKEND_H
#define RTWEEKEND_H

#include <cmath>
#include <iostream>
#include <limits>
#include <memory>
#include <vector>
#include <cstdlib>
#include <random>

// C++ Std Usings
//shared_pointer: uses reference counter 
//for multiple object every time assignment ref counter is incre
//shared pointer out of scope (block) decremented 
// if 0, object is deleted
 // using is for avoiding std::
using std::make_shared;
using std::shared_ptr;

// Constants

const double infinity = std::numeric_limits<double>::infinity();
const double pi = 3.1415926535897932385;

// Utility Functions

inline double degrees_to_radians(double degrees) {
    return degrees * pi / 180.0;
}
inline double random_double() {
    // Returns a random real in [0,1).
    return std::rand() / (RAND_MAX + 1.0);
}

inline double random_double(double min, double max) {
    // Returns a random real in [min,max).
    return min + (max-min)*random_double();
}
// Common Headers

#include "color.h"
#include "ray.h"
#include "vec3.h"
#include "interval.h"

#endif