#include "scene.hpp"

#include <iostream>

#include "terrain.hpp"
#include "utils.hpp"

using namespace cgp;

void scene_structure::initialize()
{
	// Basic set-up
	// ***************************************** //
	global_frame.initialize(mesh_primitive_frame(), "Frame");
	environment.camera.axis = camera_spherical_coordinates_axis::z;
	environment.camera.look_at({ 15.0f,6.0f,6.0f }, { 0,0,2.5f });

    environment.background_color = {0.2, 0.2, 0.2};

    terrain.initialize();
    rain.initialize();
    boat.initialize();

    for (int i = 0; i < 20; i++)
    {
        addRockGroup(10);
    }
        
}

void scene_structure::display() {
    // Basic elements of the scene
    environment.update();
    environment.camera.center_of_rotation.z = 2 + 0.9*terrain.evaluate_terrain_height(0, 0, terrain.timer.t);

    terrain.displayTerrain(environment);

    boat.update(terrain);
    boat.draw(environment);

    updateRocks();
    for (auto& rock : rocks)
        rock.draw(environment);

    display_semiTransparent();
}

void scene_structure::display_semiTransparent() {
    // Enable use of alpha component as color blending for transparent elements
    //  alpha = current_color.alpha
    //  new color = previous_color * alpha + current_color * (1-alpha)
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

    // Disable depth buffer writing
    //  - Transparent elements cannot use depth buffer
    //  - They are supposed to be display from furest to nearest elements
    glDepthMask(false);

    rain.draw(environment);

    glDepthMask(true);
    glDisable(GL_BLEND);
}

void scene_structure::display_gui() {
    ImGui::Checkbox("Frame", &gui.display_frame);
    ImGui::Checkbox("Wireframe", &gui.display_wireframe);
}

void scene_structure::addRockGroup(float minDistance) {
    int groupSize = 2+std::floor(random() * 5); // Random number in [2,6]
    float groupDist = minDistance + random() * (rocksMaxDist - minDistance);
    vec3 groupPos = groupDist * vec3(randUnitVec2(), 0);
    float spread = 3;
    for (int i = 0; i < groupSize; i++)
    {
        float radius = random() * 3;
        Rock newRock(vec3(groupPos) + randVec3() * spread, radius);
        newRock.initialize();
        rocks.push_back(newRock);
    }
}

void scene_structure::updateRocks() {
    vec3 boatDirection = boat.getBoatRotation() * vec3(1, 0, 0);

    std::vector<Rock> newRocks;
    for (auto& rock : rocks) {
        if (magnitude(rock.position) <= rocksMaxDist) {
            Rock r = rock;
            r.position -= 0.03 * vec3(boatDirection.xy(),0);
            newRocks.push_back(r);
        }
    }
    rocks = newRocks;
    while (rocks.size() < minRocks) {
        addRockGroup(30);
    }
}