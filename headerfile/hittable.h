#ifndef HITTABLE_H
#define HITTABLE_H

#include "rtweekend.h"

// this is for inheritance when we have other classes like triangle sphere 
// we dont need if for each class to call hit 
//
class hit_record {
  public:
    point3 p; // point3 and vec3 are the same class ()
    vec3 normal; 
    double t;
    bool front_face;

    void set_face_normal(const ray& r, const vec3& outward_normal) {
        // Sets the hit record normal vector.
        // NOTE: the parameter `outward_normal` is assumed to have unit length.

        front_face = dot(r.direction(), outward_normal) < 0;
        normal = front_face ? outward_normal : -outward_normal;
    }
};

class hittable {
  public:
    virtual ~hittable() = default;
    virtual bool hit(const ray& r, interval ray_t, hit_record& rec) const = 0;
};
#endif