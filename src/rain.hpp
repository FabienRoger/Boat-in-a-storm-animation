#pragma once

#include <vector>

#include "cgp/cgp.hpp"

using namespace cgp;

struct Rain {
    // Number of rain billboards
    int nbRain = 500;

    // Size of the billboard
    float size = 10.0f;
    float hsize = 2.0f;

    std::vector<vec3> positions;

    cgp::timer_basic timer;
    std::vector<float> rainOffset;  // time offset of each billboard

    cgp::mesh_drawable mesh_drawable;

    void initialize();
    void draw(cgp::scene_environment_basic_camera_spherical_coords& env);
};