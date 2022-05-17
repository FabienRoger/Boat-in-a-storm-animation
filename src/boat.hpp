#pragma once

#include <vector>

#include "cgp/cgp.hpp"
#include "terrain.hpp"
#include "stormEnvironment.hpp"

using namespace std;
using namespace cgp;

class Boat {
   public:
    // Boat position parameters
    float heightAboveWater = 0.2;
    float kSpring = 0.1;
    float dt = 0.5;
    float friction = 0.1;
    float g = 0.03;

    int nbFloaters = 4;
    vector<vec3> floatersPosition = {
        vec3(0,0,0), vec3(1,0,0), vec3(-1,0,0), vec3(0,sqrt(3),0)
    };
    vector<vec3> floatersSpeed;

    int boatFloater = 0;

    // Sait parameters
    int nbVertical = 10;
    int nbHorizontal = 10;
    vec3 wind = vec3(0, 0.05, 0);
    float sailFriction = 0.2;
    float kSailSpring = 1.3;

    vec3 sailLowPos = vec3(-1.2f, 0.0f, 1.2f);
    vec3 sailHighPos = vec3(1.0f, 0.0f, 2.7f);  // relative to low pos
    vec3 sailEndPos = vec3(-3.0f, 0.0f, 0.5f);  // relative to low pos

    vector<vector<vec3>> sailPositions;
    vector<vector<vec3>> sailSpeeds;
    cgp::mesh_drawable sail_mesh_drawable;
    cgp::mesh sail_mesh;

    cgp::timer_basic timer;
    cgp::mesh_drawable boat_mesh;
    void initialize();
    void draw(StormEnvironment& env);
    void update(Terrain& terrain);
    void update_sail(const vec3& position, const mat3& rotation);
    void generateStartSail(const vec3& position, const mat3& rotation);
};