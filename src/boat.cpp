
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

	sailPositions.resize(nbVertical, vector<vec3>(nbHorizontal));
}

void Boat::draw(cgp::scene_environment_basic_camera_spherical_coords& env)
{
	boat_mesh.transform.translation = { position.x, position.y,position.z };
	cgp::draw(boat_mesh, env);
}

void Boat::update(Terrain& terrain){
	float desiredZ = heightAboveWater + terrain.evaluate_terrain_height(position.x, position.y, terrain.timer.t);
	speed += vec3(0, 0, std::max((desiredZ - position.z) * kSpring, 0.f));
	speed += vec3(0, 0, - g);
	speed *= 1 - friction;
	position += speed * dt;
}

void Boat::update_sail()
{

}
