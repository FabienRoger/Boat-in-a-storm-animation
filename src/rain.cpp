
#include "rain.hpp"
#include "cgp/cgp.hpp"

using namespace cgp;

void Rain::initialize() {
	auto texture = opengl_load_texture_image("assets/rain.png", GL_REPEAT,
		GL_REPEAT);
	float size = 10.0f;
	mesh quad_mesh = mesh_primitive_quadrangle({ -size,0,0 }, { size,0,0 }, { size,0,2 * size }, { -size,0,2 * size });
	mesh_drawable.initialize(quad_mesh);
	// mesh_drawable.shading.color = { 0.6f,0.85f,0.5f };
	// mesh_drawable.shading.phong = { 0.4f, 0.6f,0,1 };
	mesh_drawable.texture = texture;
}

void Rain::draw(scene_environment_basic_camera_spherical_coords& environment) {
	vec3 const front = normalize(environment.camera.front() * vec3 { 1, 1, 0.01 }); // front-vector of the camera without z-component
	vec3 const right = environment.camera.right();
	// Rotation such that R*{1,0,0} = right-direction, R*{0,1,0} = front-direction
	rotation_transform R = rotation_transform::between_vector({ 1,0,0 }, { 0,1,0 }, right, front);
	mesh_drawable.transform.rotation = R;

	cgp::draw(mesh_drawable, environment);
}