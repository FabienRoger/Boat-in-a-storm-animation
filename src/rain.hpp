#pragma once

#include "cgp/cgp.hpp"

class Rain {
public:
	cgp::timer_basic timer;
	cgp::mesh_drawable mesh_drawable;
	void initialize();
	void draw(cgp::scene_environment_basic_camera_spherical_coords& env);
};