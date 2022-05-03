#pragma once

#include <vector>

#include "cgp/cgp.hpp"

using namespace std;
using namespace cgp;

class Rain {
public:
	int nbRain = 500;

	cgp::timer_basic timer;
	cgp::mesh_drawable mesh_drawable;
	vector<vec3> positions;
	vector<float> rainOffset;
	void initialize();
	void draw(cgp::scene_environment_basic_camera_spherical_coords& env);
};