
#include "boat.hpp"
#include "cgp/cgp.hpp"
#include "utils.hpp"

#include <vector>

using namespace cgp;

void Boat::initialize()
{
	mesh boatMesh = mesh_load_file_obj("assets/boat.obj");
	mesh_drawable.initialize(boatMesh, "boat mesh");
	mesh_drawable.transform.scaling = .08f;
	mesh_drawable.transform.translation = { -0,0,0.5f };
}

void Boat::draw(cgp::scene_environment_basic_camera_spherical_coords& env)
{
	cgp::draw(mesh_drawable, env);
}
