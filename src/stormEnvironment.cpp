
#include "stormEnvironment.hpp"

#include <iostream>
#include <vector>

#include "cgp/cgp.hpp"
#include "utils.hpp"

using namespace cgp;

void opengl_uniform(GLuint shader, StormEnvironment const& environment) {
    opengl_uniform(shader, "projection", environment.projection.matrix());
    opengl_uniform(shader, "view", environment.camera.matrix_view());
    opengl_uniform(shader, "light", environment.light, false);
    opengl_uniform(shader, "lightIntensity", environment.lightIntensity);
    opengl_uniform(shader, "lightningDirection", environment.lightningDirection);
    opengl_uniform(shader, "lightDirection", environment.lightDirection);
}

vec3 StormEnvironment::actualBackgroundColor() {
    return background_color * (1 - lightIntensity) + vec3(1, 1, 1) * lightIntensity;
}

void StormEnvironment::update() {
    timer.update();
    float t = std::fmod(timer.t, 10);
    lightIntensity = t < 0.2 ? (1 - 5 * t) : 0;
    // lightIntensity = t < 0.5 ? 1 : 0;
}
