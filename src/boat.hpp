#pragma once

#include <vector>

#include "cgp/cgp.hpp"
#include "stormEnvironment.hpp"
#include "terrain.hpp"

using namespace cgp;

struct Boat {
    // Boat position parameters (physics of the floaters)
    float heightAboveWater = 0.2;
    float kSpring = 0.2;
    float dt = 0.5;
    float friction = 0.3;
    float g = 0.1;

    // Floaters data
    int nbFloaters = 4;
    std::vector<vec3> floatersPosition = {
        vec3(0, 0, 0), vec3(-2, -2, 0), vec3(2, -2, 0), vec3(-2, 2, 0)};
    vec3 delta1Base = vec3(1, 0, 0);  // vec3(0, sqrt(3), 0) - vec3(1, 0, 0);
    vec3 delta2Base = vec3(0, 1, 0);  // vec3(-1, 0, 0) - vec3(1, 0, 0);
    std::vector<vec3> floatersSpeed;
    cgp::mesh_drawable floaterSphere;

    int boatFloater = 0;  // id of the floater that represents the position of the boat

    // Sail parameters
    int nbVertical = 10;
    int nbHorizontal = 10;
    float sailFriction = 0.2;
    float kSailSpring = 1.3;

    vec3 sailLowPos = vec3(-1.2f, 0.0f, 1.2f);  // relative to the boat's center
    vec3 sailHighPos = vec3(1.0f, 0.0f, 2.7f);  // relative to low pos
    vec3 sailEndPos = vec3(-3.0f, 0.0f, 0.5f);  // relative to low pos

    // Sail data
    std::vector<std::vector<vec3>> sailPositions;
    std::vector<std::vector<vec3>> sailSpeeds;
    cgp::mesh_drawable sail_mesh_drawable;
    cgp::mesh sail_mesh;

    // Wind parameters
    cgp::timer_basic windTimer;
    vec3 wind0 = vec3(0, 0.05, 0);
    vec3 wind1 = vec3(0.01, 0.03, 0);
    vec3 wind2 = vec3(-0.01, 0.02, 0);
    float windPeriod1 = 10;
    float windPeriod2 = 11;
    vec3 wind;

    // position relative to the rocks & the waves
    // it is "fake" because it's not the boat that is moving, it is the rest
    float boatFakeSpeed = 0.03;
    vec3 boatFakePos = vec3(0, 0, 0);

    cgp::mesh_drawable boat_mesh;

    void initialize();
    void draw(StormEnvironment& env);

    void updateFloaters(Terrain& terrain);
    mat3 getBoatRotation();

    void generateStartSail(const vec3& position, const mat3& rotation);
    void update_sail(const vec3& position, const mat3& rotation);

    void move(inputs_interaction_parameters inputs);
    void rotateFloaters(float angle);

    vec3 getFakeSpeed();
};