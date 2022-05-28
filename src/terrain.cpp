// Display the ocean

#include "terrain.hpp"

#include "cgp/cgp.hpp"
#include "utils.hpp"

using namespace cgp;

void Terrain::initialize() {
    // Initialization
    create_empty_terrain();
    mesh_drawable.initialize(terrain_mesh, "terrain");
    auto shader = opengl_load_shader("shaders/ocean/vert.glsl", "shaders/ocean/frag.glsl");
    mesh_drawable.shader = shader;
    mesh_drawable.shading.phong.specular = 0.8f;
    mesh_drawable.shading.color = {0.1f, 0.1f, 0.9f};
}

float Terrain::evaluate_terrain_height(float x, float y, float t, vec3& fakePos) {
    // Returns the height of the point (x,y) at t, with a shift of fakePos
    float h_0 = 5.0f;
    float omega = 0.1f;
    float A = 0.2f;
    float B = 1.0f;

    x += fakePos.x;
    y += fakePos.y;
    float X = -0.5 * x - 0.5 * y;

    float z = h_0 - h_0 * std::max(std::abs(std::cos(X * omega + A * t)), 0.05f) + B * noise_perlin({0.3 * x, 0.3 * y, 0.5 * t}, 3, 0.02f, 5.0f);  // x octave persistency, frequency gain
    return z;
}

void Terrain::displayTerrain(environmentType const& environment, vec3& fakePos) {
    //Displays the ocean
    timer.update();
    update_terrain_mesh(timer.t, fakePos);
    draw(mesh_drawable, environment); 
}

void Terrain::update_terrain_mesh(float t, vec3& fakePos) {
    // Updates the mesh and applies it to the drawable
    int N = N_terrain_samples;
    for (int ku = 0; ku < N; ++ku) {
        for (int kv = 0; kv < N; ++kv) {
            int k = kv + N * ku;
            float x = terrain_mesh.position[k].x;
            float y = terrain_mesh.position[k].y;

            float z = evaluate_terrain_height(x, y, t, fakePos);

            terrain_mesh.position[k].z = z;
        }
    }
    terrain_mesh.compute_normal();                         // Updating normals
    mesh_drawable.update_position(terrain_mesh.position);  
    mesh_drawable.update_normal(terrain_mesh.normal);      
}

void Terrain::create_empty_terrain() {
    // Creates a N by N mesh with a strating height of 0
    // Using mesh_primitive_grid use a little bit more ressources

    int N = N_terrain_samples;
    terrain_mesh.position.resize(N * N);
    terrain_mesh.uv.resize(N * N);

    for (int ku = 0; ku < N; ++ku) {
        for (int kv = 0; kv < N; ++kv) {
            float u = ku / (N - 1.0f);
            float v = kv / (N - 1.0f);

            float x = (u - 0.5f) * terrain_length + random();
            float y = (v - 0.5f) * terrain_length + random();

            terrain_mesh.position[kv + N * ku] = {x, y, 0.0f};
            terrain_mesh.uv[kv + N * ku] = {20 * u, 20 * v};
        }
    }

    // Creating triangles:
    for (int ku = 0; ku < N - 1; ++ku) {
        for (int kv = 0; kv < N - 1; ++kv) {
            unsigned int idx = kv + N * ku;

            uint3 triangle_1 = {idx, idx + 1 + N, idx + 1};
            uint3 triangle_2 = {idx, idx + N, idx + 1 + N};

            terrain_mesh.connectivity.push_back(triangle_1);
            terrain_mesh.connectivity.push_back(triangle_2);
        }
    }

    terrain_mesh.fill_empty_field();
}
