
#include "rain.hpp"
#include "cgp/cgp.hpp"
#include "utils.hpp"

#include <vector>

using namespace cgp;

void Rain::initialize() {
	auto texture = opengl_load_texture_image("assets/rain.png", GL_REPEAT,
		GL_REPEAT);
	float size = 10.0f;
	float hsize = 2.0f;
	mesh quad_mesh = mesh_primitive_quadrangle({ -hsize,0,0 }, { hsize,0,0 }, { hsize,0,2 * size }, { -hsize,0,2 * size });
	mesh_drawable.initialize(quad_mesh);
	// mesh_drawable.shading.color = { 0.6f,0.85f,0.5f };
	// mesh_drawable.shading.phong = { 0.4f, 0.6f,0,1 };
	mesh_drawable.texture = texture;

	positions.resize(nbRain);
	rainOffset.resize(nbRain);
	for (int i = 0; i < nbRain; i++)
	{
		positions[i] = 20 * (0.5 + random()) * vec3(randUnitVec2(),0);
		rainOffset[i] = random();
	}
	

}

void Rain::draw(scene_environment_basic_camera_spherical_coords& environment) {
	vec3 const front = normalize(environment.camera.front() * vec3 { 1, 1, 0.01 }); // front-vector of the camera without z-component
	vec3 const right = environment.camera.right();
	// Rotation such that R*{1,0,0} = right-direction, R*{0,1,0} = front-direction
	rotation_transform R = rotation_transform::between_vector({ 1,0,0 }, { 0,1,0 }, right, front);
	mesh_drawable.transform.rotation = R;

	timer.update();
	float t = timer.t;

	for (int i = 0; i < nbRain; i++)
	{
		mesh_drawable.transform.translation = vec3(environment.camera.position().xy(), 0) + positions[i];
		float deltay = std::fmod(t + rainOffset[i], 1);
		mesh_drawable.update_uv({ {0,deltay},{1,deltay},{1,1 + deltay},{0,1 + deltay} });
		cgp::draw(mesh_drawable, environment);
	}
	cgp::draw(mesh_drawable, environment);
}