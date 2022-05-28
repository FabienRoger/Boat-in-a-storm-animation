
#include "stormEnvironment.hpp"

#include <iostream>
#include <vector>

#include "cgp/cgp.hpp"
#include "utils.hpp"

using namespace cgp;

// Overrite the opengl_uniform for this custom environment
void opengl_uniform(GLuint shader, StormEnvironment const& environment) {
    opengl_uniform(shader, "projection", environment.projection.matrix());
    opengl_uniform(shader, "view", environment.camera.matrix_view());
    opengl_uniform(shader, "light", environment.light, false);

    // Additional properties
    opengl_uniform(shader, "lightIntensity", environment.lightIntensity);
    opengl_uniform(shader, "lightningDirection", environment.lightningDirection);
    opengl_uniform(shader, "lightDirection", environment.lightDirection);
}

vec3 StormEnvironment::actualBackgroundColor() {
    return background_color * (1 - lightIntensity) + vec3(1, 1, 1) * lightIntensity;
}

void StormEnvironment::update() {
    timer.update();
    float t = timer.t;

    if (t > timeToNextLight) {
        // Reset the timer
        lightUp();
    }

    lightIntensity = t < 0.2 ? (1 - 5 * t) : 0;
}

void StormEnvironment::lightUp() {
    timer.t = 0;
    timeToNextLight = random() * 5;
}