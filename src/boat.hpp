#pragma once

#include <vector>

#include "cgp/cgp.hpp"

using namespace std;
using namespace cgp;

class Boat {
public:
	int nbVertical = 5;
	int nbHorizontal = 5;
	cgp::timer_basic timer;
	cgp::mesh_drawable boat_mesh;
	vector<vector<vec3>> sailPositions;
	void initialize();
	void draw(cgp::scene_environment_basic_camera_spherical_coords& env);
	void update_sail();
};