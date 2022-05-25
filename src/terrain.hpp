#pragma once

#include "cgp/cgp.hpp"
#include "stormEnvironment.hpp"

class Terrain {
   public:
    using environmentType = StormEnvironment;
    cgp::mesh_drawable mesh_drawable;
    cgp::mesh terrain_mesh;
    int N_terrain_samples = 80;
    float terrain_length = 100;
    cgp::timer_basic timer;

    void initialize();

    float evaluate_terrain_height(float x, float y, float t, vec3& fakePos);

    void update_terrain_mesh(float t, vec3& fakePos);

    void displayTerrain(environmentType const& environment, vec3& fakePos);
    void create_empty_terrain();
    int logger = 0;
};