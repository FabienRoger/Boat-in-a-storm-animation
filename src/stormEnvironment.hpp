#pragma once

#include <vector>

#include "cgp/cgp.hpp"

using namespace std;
using namespace cgp;

class StormEnvironment : public scene_environment_basic_camera_spherical_coords {
   public:
    float lightIntensity = 0.5;
    vec3 lightningDirection = vec3(1, 0, 0);
    vec3 actualBackgroundColor();
    void update();
    cgp::timer_basic timer;
};

void opengl_uniform(GLuint shader, StormEnvironment const& scene_environment);