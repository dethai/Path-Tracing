
#include "headerfile/rtweekend.h"
#include "headerfile/hittable.h"
#include "headerfile/hittable_list.h"
#include "headerfile/material.h"
#include "headerfile/sphere.h"
#include "headerfile/camera.h"
#include "headerfile/timing.h"

#include <mpi.h>
#include <vector>
#include <iostream>
#include <algorithm>

int main(int argc, char **argv) {
    

   
    MPI_Init(&argc, &argv);

    int rank, size;
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    MPI_Comm_size(MPI_COMM_WORLD, &size);

    //std::cerr << "[Rank " << rank << "] started\n";

    // -------------------------
    // WORLD
    // -------------------------
    hittable_list world;

    auto ground_material = make_shared<lambertian>(color(0.5, 0.5, 0.5));
    world.add(make_shared<sphere>(point3(0,-1000,0), 1000, ground_material));
     for (int a = -5; a < 5; a++) {
        for (int b = -11; b < 11; b++) {
            auto choose_mat = random_double();
            point3 center(a + 0.9*random_double(), 0.2, b + 0.9*random_double());

            if ((center - point3(4, 0.2, 0)).length() > 0.9) {
                shared_ptr<material> sphere_material;

                if (choose_mat < 0.8) {
                    // diffuse
                    auto albedo = color::random() * color::random();
                    sphere_material = make_shared<lambertian>(albedo);
                    world.add(make_shared<sphere>(center, 0.2, sphere_material));
                } else if (choose_mat < 0.95) {
                    // metal
                    auto albedo = color::random(0.5, 1);
                    auto fuzz = random_double(0, 0.5);
                    sphere_material = make_shared<metal>(albedo, fuzz);
                    world.add(make_shared<sphere>(center, 0.2, sphere_material));
                } else {
                    // glass
                    sphere_material = make_shared<dielectric>(1.5);
                    world.add(make_shared<sphere>(center, 0.2, sphere_material));
                }
            }
        }
    }
    auto material1 = make_shared<dielectric>(1.5);
    world.add(make_shared<sphere>(point3(0, 1, 0), 1.0, material1));

    auto material2 = make_shared<lambertian>(color(0.4, 0.2, 0.1));
    world.add(make_shared<sphere>(point3(-4, 1, 0), 1.0, material2));

    auto material3 = make_shared<metal>(color(0.7, 0.6, 0.5), 0.0);
    world.add(make_shared<sphere>(point3(4, 1, 0), 1.0, material3));

    camera cam;
    cam.aspect_ratio = 16.0 / 9.0;
    cam.image_width = 1200;
    cam.samples_per_pixel = 500;
    cam.max_depth = 50;
    cam.vfov = 20;
    cam.lookfrom = point3(13,2,3);
    cam.lookat   = point3(0,0,0);
    cam.vup      = vec3(0,1,0);
    cam.defocus_angle = 0.6;
    cam.focus_dist = 10.0;

    int image_height = cam.get_image_height();

    // -------------------------
    // ROW SPLIT (SAFE VERSION)
    // -------------------------
    int base = image_height / size;
    int rem  = image_height % size;

    int start = rank * base + std::min(rank, rem);
    int local_height = base + (rank < rem ? 1 : 0);
    int end = start + local_height;

    int local_pixels = local_height * cam.image_width;
    /*
    std::cerr << "[Rank " << rank << "] start=" << start
              << " end=" << end
              << " local_height=" << local_height << "\n";
    */
    // -------------------------
    // BUFFER ALLOCATION
    // -------------------------
    std::vector<double> local_buffer(local_pixels * 3);
    std::vector<double> global_buffer;

    if (rank == 0)
        global_buffer.resize(image_height * cam.image_width * 3);

    // -------------------------
    // RENDER
    // -------------------------
    MPI_Barrier(MPI_COMM_WORLD);
    double t0 = MPI_Wtime();
    cam.render(world, start, end, local_buffer);

    // -------------------------
    // DEBUG: LOCAL BUFFER CHECK
    // -------------------------
    if (!local_buffer.empty()) {
        /*
        std::cerr << "[Rank " << rank << "] first pixel = "
                  << local_buffer[0] << " "
                  << local_buffer[1] << " "
                  << local_buffer[2] << "\n";
        */
    }

    // -------------------------
    // SANITY CHECK
    // -------------------------
    if ((int)local_buffer.size() != local_pixels * 3) {
        //std::cerr << "[Rank " << rank << "] BUFFER SIZE MISMATCH!\n";
        MPI_Abort(MPI_COMM_WORLD, 1);
    }

    // -------------------------
    // MPI LAYOUT CHECK
    // -------------------------
    std::vector<int> recvcounts(size);
    std::vector<int> displs(size);

    for (int r = 0; r < size; r++) {
        int rows = base + (r < rem ? 1 : 0);
        recvcounts[r] = rows * cam.image_width * 3;
    }

    displs[0] = 0;
    for (int r = 1; r < size; r++) {
        displs[r] = displs[r-1] + recvcounts[r-1];
    }

    // print layout once
    if (rank == 0) {
        std::cerr << "=== MPI LAYOUT ===\n";
        for (int r = 0; r < size; r++) {
        /*
            std::cerr << "rank " << r
                      << " recv=" << recvcounts[r]
                      << " disp=" << displs[r] << "\n";
        */
        }
    }

    // -------------------------
    // GATHER
    // -------------------------
    MPI_Gatherv(
        local_buffer.data(),
        local_pixels * 3,
        MPI_DOUBLE,
        global_buffer.data(),
        recvcounts.data(),
        displs.data(),
        MPI_DOUBLE,
        0,
        MPI_COMM_WORLD
    );
    MPI_Barrier(MPI_COMM_WORLD);
    double t1 = MPI_Wtime();
    // -------------------------
    // OUTPUT CHECK (ONLY ROOT)
    // -------------------------
    if (rank == 0) {
        /*
        std::cout << "P3\n"
                  << cam.image_width << " " << image_height << "\n"
                  << "255\n";

        // verify first few pixels BEFORE writing ppm
        std::cerr << "GLOBAL sanity check:\n";
        std::cerr << global_buffer[0] << " "
                  << global_buffer[1] << " "
                  << global_buffer[2] << "\n";
        */
        std::cout << "MPI Render Time: " << (t1 - t0) << " seconds\n";
        for (int i = 0; i < image_height * cam.image_width; i++) {
            color pixel(
                global_buffer[i*3 + 0],
                global_buffer[i*3 + 1],
                global_buffer[i*3 + 2]
            );
            write_color(std::cout, pixel);
        }
    }

    //std::cerr << "[Rank " << rank << "] finalize\n";
    MPI_Finalize();
}