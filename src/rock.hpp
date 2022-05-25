#pragma once

#include "cgp/cgp.hpp"
#include "stormEnvironment.hpp"

struct Rock {
    vec3 position;
    float radius;

    mesh rockMesh;
    mesh_drawable rockMeshDrawable;

    Rock();
    Rock(vec3 position, float radius);

    void initialize(GLuint shader);
    void draw(StormEnvironment const& environment);

    // Transformation to make the rock more interesting than a sphere
    void cut(float cutLevel);
    void addPerlinNoise(float amplitude);
};