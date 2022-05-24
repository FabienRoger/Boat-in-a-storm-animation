#include "scene.hpp"

#include <iostream>

#include "terrain.hpp"

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
}

void scene_structure::display() {
    // Basic elements of the scene
    environment.update();
    environment.camera.center_of_rotation.z = 2 + 0.9*terrain.evaluate_terrain_height(0, 0, terrain.timer.t);

    terrain.displayTerrain(environment);

    boat.update(terrain);
    boat.draw(environment);

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
