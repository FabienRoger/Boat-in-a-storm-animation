#pragma once

#include <vector>

#include "cgp/cgp.hpp"

using namespace cgp;

class Rain {
   public:
    int nbRain = 500;

    cgp::timer_basic timer;
    cgp::mesh_drawable mesh_drawable;
    std::vector<vec3> positions;
    std::vector<float> rainOffset;
    void initialize();
    void draw(cgp::scene_environment_basic_camera_spherical_coords& env);
};