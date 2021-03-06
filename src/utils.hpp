#pragma once

#include "cgp/cgp.hpp"

using namespace cgp;

const float PI = 3.1415926;

float random();
vec3 randVec3();
vec2 randUnitVec2();

float magnitude(vec3& v);

// Get the rotation to send the plane directed by b1 and b2
// on the plane directed by a1 and a2
mat3 getRotation(vec3 a1, vec3 a2, vec3 b1, vec3 b2);

vec3 makeOrthogonal(vec3 a1, vec3 a2);