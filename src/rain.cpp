
#include "rain.hpp"

#include <vector>

#include "cgp/cgp.hpp"
#include "utils.hpp"

using namespace cgp;

void Rain::initialize() {
    auto texture = opengl_load_texture_image("assets/rain.png", GL_REPEAT,
                                             GL_REPEAT);

    mesh quad_mesh = mesh_primitive_quadrangle(
        {-hsize, 0, 0}, {hsize, 0, 0}, {hsize, 0, 2 * size}, {-hsize, 0, 2 * size});

    mesh_drawable.initialize(quad_mesh);

    auto shader = opengl_load_shader("shaders/rain/vert.glsl", "shaders/rain/frag.glsl");
    mesh_drawable.texture = texture;
    mesh_drawable.shader = shader;

    positions.resize(nbRain);
    rainOffset.resize(nbRain);

    // Put the rain billboards around the boat
    for (int i = 0; i < nbRain; i++) {
        positions[i] = 20 * random() * vec3(randUnitVec2(), 0);
        rainOffset[i] = random();
    }
}

void Rain::draw(scene_environment_basic_camera_spherical_coords& environment) {
    // front-vector of the camera without z-component
    vec3 const front = normalize(environment.camera.front() * vec3{1, 1, 0});
    vec3 const right = environment.camera.right();

    // Make so that the billboard face the camera
    // Rotation such that R*{1,0,0} = right-direction, R*{0,1,0} = front-direction
    rotation_transform R = rotation_transform::between_vector({1, 0, 0}, {0, 1, 0}, right, front);
    mesh_drawable.transform.rotation = R;

    timer.update();
    float t = timer.t;

    // Update the position, rotation and uv of each billboard
    for (int i = 0; i < nbRain; i++) {
        mesh_drawable.transform.translation = vec3(environment.camera.position().xy(), 0) + positions[i];
        float deltay = std::fmod(t + rainOffset[i], 1);
        mesh_drawable.update_uv({{0, deltay}, {1, deltay}, {1, 1 + deltay}, {0, 1 + deltay}});
        cgp::draw(mesh_drawable, environment);
    }
}