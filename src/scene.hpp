#pragma once

#include "cgp/cgp.hpp"
#include "terrain.hpp"
#include "rain.hpp"
#include "boat.hpp"
#include "stormEnvironment.hpp"
#include "rock.hpp"


// The element of the GUI that are not already stored in other structures
struct gui_parameters {
	bool display_frame      = true;
	bool display_wireframe  = false;
};



// The structure of the custom scene
struct scene_structure {
	
	// ****************************** //
	// Elements and shapes of the scene
	// ****************************** //

	cgp::mesh_drawable global_frame;          // The standard global frame
	StormEnvironment environment; // Standard environment controler
	cgp::inputs_interaction_parameters inputs; // Storage for inputs status (mouse, keyboard, window dimension)

	gui_parameters gui;                       // Standard GUI element storage

	Terrain terrain;
	Rain rain;
	Boat boat;

	int minRocks = 100;
	float rocksMaxDist = 100;
	std::vector<Rock> rocks;


	// ****************************** //
	// Functions
	// ****************************** //

	void initialize();  // Standard initialization to be called before the animation loop
	void display();     // The frame display to be called within the animation loop
	void display_gui(); // The display of the GUI, also called within the animation loop
	void display_semiTransparent();

	void addRockGroup(float minDistance);
	void updateRocks();

};





