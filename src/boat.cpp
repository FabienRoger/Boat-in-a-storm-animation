
#include "boat.hpp"
#include "cgp/cgp.hpp"
#include "utils.hpp"

#include <vector>

using namespace cgp;
using namespace std;

void Boat::initialize()
{
	mesh boatMesh = mesh_load_file_obj("assets/boat.obj");
	boat_mesh.initialize(boatMesh, "boat mesh");
	boat_mesh.transform.scaling = .08f;
	boat_mesh.transform.translation = { -0,0,0.5f };

	sailPositions.resize(nbVertical, vector<vec3>(nbHorizontal));
}

void Boat::draw(cgp::scene_environment_basic_camera_spherical_coords& env)
{
	cgp::draw(boat_mesh, env);
}

void Boat::update_sail()
{

}
