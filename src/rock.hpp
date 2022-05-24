#pragma once

#include "cgp/cgp.hpp"

#include "stormEnvironment.hpp"


class Rock {
public:
	vec3 position = vec3(-5, -2, 5);
	float radius = 3;
	mesh rockMesh;
	mesh_drawable rockMeshDrawable;

	void initialize();
	void draw(StormEnvironment const& environment);
	void improveDetails();
};