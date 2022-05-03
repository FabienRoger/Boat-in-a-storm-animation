

#pragma once

#include <vector>

#include "cgp/cgp.hpp"

using namespace std;
using namespace cgp;

// https://free3d.com/3d-model/3dbenchy-the-jolly-3d-printing-torture-test-54655.html

class Boat {
public:
	cgp::mesh_drawable mesh_drawable;
	void initialize();
	void draw(cgp::scene_environment_basic_camera_spherical_coords& env);
};