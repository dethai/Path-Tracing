#ifndef HITTABLE_LIST_H
#define HITTABLE_LIST_H

#include "hittable.h"
#include "rtweekend.h"
//shared_pointer: uses reference counter 
//for multiple object every time assignment ref counter is incre
//shared pointer out of scope (block) decremented 
// if 0, object is deleted
 // using is for avoiding std::


class hittable_list : public hittable {
  public:
    std::vector<shared_ptr<hittable>> objects;

    hittable_list() {} // creates an empty world
    hittable_list(shared_ptr<hittable> object) { add(object); }
    //adds objects
    void clear() { objects.clear(); } // removes all objects

    void add(shared_ptr<hittable> object) {
        objects.push_back(object);
    }
    //loops through all objects and calls hit (each object own hit function)
    bool hit(const ray& r, interval ray_t, hit_record& rec) const override {
        hit_record temp_rec;
        bool hit_anything = false;
        auto closest_so_far = ray_t.max;

        for (const auto& object : objects) {
            if (object->hit(r, interval(ray_t.min, closest_so_far), temp_rec)) {
                hit_anything = true;
                closest_so_far = temp_rec.t;
                rec = temp_rec;
            }
        }

        return hit_anything;
    }
};

#endif