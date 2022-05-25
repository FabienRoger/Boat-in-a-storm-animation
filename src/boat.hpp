#pragma once

#include <vector>

#include "cgp/cgp.hpp"
#include "stormEnvironment.hpp"
#include "terrain.hpp"

using namespace cgp;

class Boat {
   public:

    // Boat position parameters
    float heightAboveWater = 0.2;
    float kSpring = 0.2;
    float dt = 0.5;
    float friction = 0.3;
    float g = 0.1;

    int nbFloaters = 4;
    std::vector<vec3> floatersPosition = {
    vec3(0, 0, 0), vec3(-2, -2, 0), vec3(2, -2, 0), vec3(-2, 2, 0)
    };
    vec3 delta1Base = vec3(1, 0, 0);  // vec3(0, sqrt(3), 0) - vec3(1, 0, 0);
    vec3 delta2Base = vec3(0, 1, 0);  // vec3(-1, 0, 0) - vec3(1, 0, 0);

    std::vector<vec3> floatersSpeed;

    int boatFloater = 0;

    // Sait parameters
    int nbVertical = 10;
    int nbHorizontal = 10;

    cgp::timer_basic windTimer;
    vec3 wind0 = vec3(0, 0.05, 0);
    vec3 wind1 = vec3(0.01, 0.03, 0);
    vec3 wind2 = vec3(-0.01, 0.02, 0);
    float windPeriod1 = 10;
    float windPeriod2 = 11;
    vec3 wind;

    float sailFriction = 0.2;
    float kSailSpring = 1.3;

    vec3 sailLowPos = vec3(-1.2f, 0.0f, 1.2f);
    vec3 sailHighPos = vec3(1.0f, 0.0f, 2.7f);  // relative to low pos
    vec3 sailEndPos = vec3(-3.0f, 0.0f, 0.5f);  // relative to low pos

    float boatFakeSpeed = 0.03; // relative to the rocks
    vec3 boatFakePos = vec3(0, 0, 0);

    std::vector<std::vector<vec3>> sailPositions;
    std::vector<std::vector<vec3>> sailSpeeds;
    cgp::mesh_drawable sail_mesh_drawable;
    cgp::mesh sail_mesh;
    cgp::mesh_drawable floaterSphere;

    cgp::timer_basic timer;
    cgp::mesh_drawable boat_mesh;
    void initialize();
    void draw(StormEnvironment& env);
    void update(Terrain& terrain);
    void update_sail(const vec3& position, const mat3& rotation);
    void generateStartSail(const vec3& position, const mat3& rotation);
    mat3 getBoatRotation();
    void move(inputs_interaction_parameters inputs);
    void rotateFloaters(float angle);
    vec3 getFakeSpeed();
};