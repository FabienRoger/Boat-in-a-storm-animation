#pragma once

#include "cgp/cgp.hpp"

#include "stormEnvironment.hpp"


class Rock {
public:
	vec3 position = vec3(-5, -2, 5);
	float radius = 3;
	mesh rockMesh;
	mesh_drawable rockMeshDrawable;

	Rock();
	Rock(vec3 position, float radius);
	void initialize(GLuint shader);
	void draw(StormEnvironment const& environment);
	void cut(float cutLevel);
	void addPerlinNoise(float amplitude);
};