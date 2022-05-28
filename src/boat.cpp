
#include "boat.hpp"

#include <iostream>
#include <vector>

#include "cgp/cgp.hpp"
#include "stormEnvironment.hpp"
#include "utils.hpp"

using namespace cgp;

void Boat::initialize() {
    // Initialize the boat
    mesh boatMesh = mesh_load_file_obj("assets/boat.obj");
    boat_mesh.initialize(boatMesh, "boat mesh");
    boat_mesh.texture = opengl_load_texture_image("assets/boat.jpg");
    boat_mesh.transform.scaling = .08f;

    // Initialize the sail
    vec3 position = floatersPosition[boatFloater];
    mat3 rotation = getBoatRotation();
    generateStartSail(position, rotation);
    sail_mesh_drawable.initialize(sail_mesh, "sail");
    sail_mesh_drawable.shading.color = {1.0f, 1.0f, 1.0f};
    sail_mesh_drawable.shading.phong.specular = 0.2f;

    // Initialize the floaters
    floatersSpeed.resize(nbFloaters, vec3(0, 0, 0));
    floaterSphere.initialize(mesh_primitive_sphere(1), "floater");

    // Initialize the wind
    windTimer.start();
}

void Boat::draw(StormEnvironment& env) {
    // Translate and rotate the boat
    vec3 position = floatersPosition[boatFloater];
    boat_mesh.transform.translation = {position.x, position.y, position.z};
    mat3 rotation = getBoatRotation();
    boat_mesh.transform.rotation = rotation_transform::from_matrix(rotation);

    update_sail(position, rotation);
    
    boatFakePos += getFakeSpeed();

    // Update the boat's light
    env.light = sailPositions[nbVertical - 1][0];
    env.lightDirection = rotation * vec3(1, 0, 0);

    cgp::draw(boat_mesh, env);
    cgp::draw(sail_mesh_drawable, env);

    // Display floaters for debug
    /*
    floaterSphere.transform.translation = floatersPosition[2];
    cgp::draw(floaterSphere, env);
    floaterSphere.transform.translation = floatersPosition[1];
    cgp::draw(floaterSphere, env);
    floaterSphere.transform.translation = floatersPosition[3];
    cgp::draw(floaterSphere, env);
    */
}

void Boat::updateFloaters(Terrain& terrain) {
    // Do physics update on every floater
    for (int i = 0; i < nbFloaters; i++) {
        // Compute v(t+dt)
        float desiredZ = heightAboveWater + terrain.evaluate_terrain_height(floatersPosition[i].x, floatersPosition[i].y, terrain.timer.t, boatFakePos);
        floatersSpeed[i] += vec3(0, 0, std::max((desiredZ - floatersPosition[i].z) * kSpring, 0.f));
        floatersSpeed[i] += vec3(0, 0, -g);
        floatersSpeed[i] *= 1 - friction;
        // Compute x(t+dt)
        floatersPosition[i] += floatersSpeed[i] * dt;
    }
}

void Boat::generateStartSail(const vec3& position, const mat3& rotation) {
    int nbPoints = 0;
    sailPositions.resize(nbVertical);
    sailSpeeds.resize(nbVertical);

    // Generate the positions for a triangular sail
    for (int i = 0; i < nbVertical; i++) {
        sailPositions[i] = std::vector<vec3>(nbHorizontal - i);
        sailSpeeds[i] = std::vector<vec3>(nbHorizontal - i);
        for (int j = 0; j < nbHorizontal - i; j++) {
            vec3 verticalPos = i * sailHighPos / (nbVertical - 1);
            vec3 horizontalPos = j * sailEndPos / (nbHorizontal - 1);
            sailPositions[i][j] = rotation * (sailLowPos + verticalPos + horizontalPos) + position;
            nbPoints++;
        }
    }

    // Create the appropriate mesh
    sail_mesh.position.resize(2 * nbPoints);
    int point = 0;
    for (int i = 0; i < nbVertical; i++) {
        for (int j = 0; j < nbHorizontal - i; j++) {
            // double points to have different normals for both
            sail_mesh.position[2 * point] = sailPositions[i][j];      // point "on one side"
            sail_mesh.position[2 * point + 1] = sailPositions[i][j];  // point "on the other"
            point++;
        }
    }

    // Connect points in the mesh
    point = -1;
    for (int i = 0; i < nbVertical; i++) {
        for (int j = 0; j < nbHorizontal - i; j++) {
            point++;
            if (i == 0) continue;
            if (j == nbHorizontal - i - 1) {
                int u = point;
                int v = point - (nbHorizontal - i);
                int w = point - (nbHorizontal - i) - 1;
                sail_mesh.connectivity.push_back(
                    {2 * u, 2 * v, 2 * w});
                sail_mesh.connectivity.push_back(
                    {2 * u + 1, 2 * v + 1, 2 * w + 1});
            }
            if (j > 0) {
                int u = point;
                int v = point - (nbHorizontal - i) - 1;
                int w = point - (nbHorizontal - i) - 2;
                sail_mesh.connectivity.push_back(
                    {2 * u, 2 * v, 2 * w});
                sail_mesh.connectivity.push_back(
                    {2 * u + 1, 2 * v + 1, 2 * w + 1});

                u = point - 1;
                v = point;
                w = point - (nbHorizontal - i) - 2;

                sail_mesh.connectivity.push_back(
                    {2 * u, 2 * v, 2 * w});
                sail_mesh.connectivity.push_back(
                    {2 * u + 1, 2 * v + 1, 2 * w + 1});
            }
        }
    }

    sail_mesh.fill_empty_field();
}

void Boat::update_sail(const vec3& position, const mat3& rotation) {
    // Update wind
    windTimer.update();
    wind = wind0 + wind1 * sin(2 * PI * windTimer.t / windPeriod1) + wind2 * sin(2 * PI * windTimer.t / windPeriod2);

    // Update sail nodes' speed
    for (int i = 0; i < nbVertical; i++) {
        for (int j = 0; j < nbHorizontal - i; j++) {
            sailSpeeds[i][j] += wind;
            vec3 idealPosition = vec3(0, 0, 0);

            // center
            if (i > 0 && i < nbVertical - 1 && j > 0 && j < nbHorizontal - i - 1) {
                idealPosition += sailPositions[i - 1][j];
                idealPosition += sailPositions[i + 1][j];
                idealPosition += sailPositions[i][j - 1];
                idealPosition += sailPositions[i][j + 1];
                idealPosition /= 4;
            }
            // diagonal
            else if (j == nbHorizontal - i - 1 && i > 0 && i < nbVertical - 1 && j > 0) {
                idealPosition += sailPositions[i - 1][j + 1];
                idealPosition += sailPositions[i + 1][j - 1];
                idealPosition /= 2;
            }
            // left
            else if (i == 0 && i < nbVertical - 1 && j > 0 && j < nbHorizontal - i - 1) {
                idealPosition += sailPositions[i][j - 1];
                idealPosition += sailPositions[i][j + 1];
                idealPosition /= 2;
            }
            // down
            else if (i > 0 && i < nbVertical - 1 && j == 0 && j < nbHorizontal - i - 1) {
                idealPosition += sailPositions[i - 1][j];
                idealPosition += sailPositions[i + 1][j];
                idealPosition /= 2;
            }

            sailSpeeds[i][j] += kSailSpring * (idealPosition - sailPositions[i][j]);

            sailSpeeds[i][j] *= 1 - sailFriction;
        }
    }

    // Update x
    for (int i = 0; i < nbVertical; i++) {
        for (int j = 0; j < nbHorizontal - i; j++) {
            sailPositions[i][j] += dt * sailSpeeds[i][j];
        }
    }

    // Fix end points
    sailPositions[0][0] = rotation * sailLowPos + position;
    sailPositions[nbVertical - 1][0] = rotation * (sailHighPos + sailLowPos) + position;
    sailPositions[0][nbHorizontal - 1] = rotation * (sailEndPos + sailLowPos) + position;

    // Update display of sail
    int point = 0;
    for (int i = 0; i < nbVertical; i++) {
        for (int j = 0; j < nbHorizontal - i; j++) {
            sail_mesh.position[2 * point] = sailPositions[i][j];
            sail_mesh.position[2 * point + 1] = sailPositions[i][j];
            point++;
        }
    }

    sail_mesh.compute_normal();                              // Ne pas oublier les normales du mesh évoluent - PI
    sail_mesh_drawable.update_position(sail_mesh.position);  // Mise à jour des positions - PI
    sail_mesh_drawable.update_normal(sail_mesh.normal);      // et des normales. - PI
}

mat3 Boat::getBoatRotation() {
    // Find the rotation that puts the base vectors in the same plane as the floater's plane
    vec3 position = floatersPosition[boatFloater];
    vec3 delta1 = floatersPosition[2] - floatersPosition[1];
    vec3 delta2 = floatersPosition[3] - floatersPosition[1];
    return getRotation(delta1, delta2, delta1Base, delta2Base);
}

void Boat::move(inputs_interaction_parameters inputs) {
    inputs_keyboard_parameters& keyboard = inputs.keyboard;

    float rotationSpeed = 0.03;
    float acceleration = 1.05;

    if (keyboard.up)
        boatFakeSpeed *= acceleration;
    if (keyboard.down)
        boatFakeSpeed /= acceleration;
    if (keyboard.right)
        rotateFloaters(-rotationSpeed);
    if (keyboard.left)
        rotateFloaters(rotationSpeed);

    boatFakePos += getFakeSpeed();
}

vec3 Boat::getFakeSpeed() {
    vec3 boatDirection = getBoatRotation() * vec3(1, 0, 0);
    return boatFakeSpeed * vec3(boatDirection.xy(), 0);
}

void Boat::rotateFloaters(float angle) {
    auto rotation = rotation_transform::from_axis_angle(vec3(0, 0, 1), angle);
    for (int i = 0; i < nbFloaters; i++) {
        floatersPosition[i] = rotation * floatersPosition[i];
    }
}