#include "utils.hpp"

#include <cstdlib>

#include "cgp/cgp.hpp"

using namespace std;
using namespace cgp;

float random() {
	return rand() * 1.0f / RAND_MAX;
	randVec3();
}

vec3 randVec3() {
	return vec3(random(), random(), random());
}

vec2 randUnitVec2() {
	double phi = 2.0 * PI * random();
	return vec2(cos(phi), sin(phi));
}