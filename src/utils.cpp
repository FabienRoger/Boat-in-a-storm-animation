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

mat3 getRotation(vec3 a1, vec3 a2, vec3 b1, vec3 b2) {
	a1 = normalize(a1);
	a2 = normalize(a2);
	b1 = normalize(b1);
	b2 = normalize(b2);

	a2 = makeOrthogonal(a1, a2);
	b2 = makeOrthogonal(b1, b2);

	vec3 a3 = cross(a1, a2);
	vec3 b3 = cross(b1, b2);

	mat3 A;
	A[0] = a1;
	A[1] = a2;
	A[2] = a3;

	mat3 B;
	B[0] = b1;
	B[1] = b2;
	B[2] = b3;

	return B * inverse(A);
}

vec3 makeOrthogonal(vec3 a1, vec3 a2) {
	vec3 projected = dot(a1, a2) * a1;
	return normalize(a2 - projected);
}