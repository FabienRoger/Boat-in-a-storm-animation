#pragma once

#include <vector>

#include "cgp/cgp.hpp"
#include "terrain.hpp"

using namespace std;
using namespace cgp;

class Boat {
public:
	int nbVertical = 5;
	int nbHorizontal = 5;

	float heightAboveWater = 0;
	vec3 position = vec3(0,0,0);
	vec3 speed = vec3(0, 0, 0);
	float kSpring = 0.1;
	float dt = 1;
	float friction = 0.1;
	float g = 0.03;

	cgp::timer_basic timer;
	cgp::mesh_drawable boat_mesh;
	vector<vector<vec3>> sailPositions;
	void initialize();
	void draw(cgp::scene_environment_basic_camera_spherical_coords& env);
	void update(Terrain& terrain);
	void update_sail();
};