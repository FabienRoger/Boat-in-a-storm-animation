
#include "stormEnvironment.hpp"
#include "cgp/cgp.hpp"
#include "utils.hpp"

#include <vector>
#include <iostream>

using namespace cgp;
using namespace std;

void opengl_uniform(GLuint shader, StormEnvironment const& environment) {
	opengl_uniform(shader, "projection", environment.projection.matrix());
	opengl_uniform(shader, "view", environment.camera.matrix_view());
	opengl_uniform(shader, "light", environment.light, false);
	opengl_uniform(shader, "lightIntensity", environment.lightIntensity);
}

vec3 StormEnvironment::actualBackgroundColor()
{
	return background_color * (1 - lightIntensity) + vec3(1,1,1) * lightIntensity;
}

void StormEnvironment::update() {
	timer.update();
	float t = std::fmod(timer.t, 60);
	lightIntensity = t < 0.2 ? (1 - 5 * t) : 0;
}
