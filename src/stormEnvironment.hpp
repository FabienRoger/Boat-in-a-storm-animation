#pragma once

#include <vector>

#include "cgp/cgp.hpp"

using namespace cgp;

struct StormEnvironment : public scene_environment_basic_camera_spherical_coords {
    // Properties of the lightning
    float lightIntensity = 0.5;
    float timeToNextLight = 5;
    vec3 lightningDirection = vec3(1, 0, 0);
    vec3 lightDirection = vec3(1, 0, 0);

    cgp::timer_basic timer;

    // get background color adjusted for if there is currently lightning or not
    vec3 actualBackgroundColor();

    void update();

    void lightUp();
};

void opengl_uniform(GLuint shader, StormEnvironment const& scene_environment);