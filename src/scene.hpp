#pragma once

#include "boat.hpp"
#include "cgp/cgp.hpp"
#include "rain.hpp"
#include "rock.hpp"
#include "stormEnvironment.hpp"
#include "terrain.hpp"

struct scene_structure {
    cgp::mesh_drawable global_frame;            // The standard global frame
    StormEnvironment environment;               // Custom environment controler
    cgp::inputs_interaction_parameters inputs;  // Storage for inputs status (mouse, keyboard, window dimension)

    Terrain terrain;
    Rain rain;
    Boat boat;

    // Rock-related properties
    GLuint rockShader;
    int minRocks = 100;
    float rocksMaxDist = 100;
    // The rocks themselves
    std::vector<Rock> rocks;

    void initialize();  // Standard initialization to be called before the animation loop
    void display();     // The frame display to be called within the animation loop
    void display_semiTransparent();

    void addRockGroup(float minDistance);
    void updateRocks();
};
