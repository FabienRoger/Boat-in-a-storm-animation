#pragma once

#include "cgp/cgp.hpp"
#include "stormEnvironment.hpp"

struct Terrain {
    using environmentType = StormEnvironment;

    // Properties of the terrain
    int N_terrain_samples = 80;
    float terrain_length = 100;

    // Terrain data
    cgp::mesh_drawable mesh_drawable;
    cgp::mesh terrain_mesh;

    // Wave timer
    cgp::timer_basic timer;

    void initialize();
    void create_empty_terrain();

    // The fake position of the boat is passed as an argument so that the relative position
    // that we care about can be computed here
    float evaluate_terrain_height(float x, float y, float t, vec3& fakePos);
    void update_terrain_mesh(float t, vec3& fakePos);
    void displayTerrain(environmentType const& environment, vec3& fakePos);
};