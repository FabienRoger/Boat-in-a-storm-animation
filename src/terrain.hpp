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

	float evaluate_terrain_height(float x, float y, float t);

	/** Compute a terrain mesh
		The (x,y) coordinates of the terrain are set in [-length/2, length/2].
		The z coordinates of the vertices are computed using evaluate_terrain_height(x,y).
		The vertices are sampled along a regular grid structure in (x,y) directions.
		The total number of vertices is N*N (N along each direction x/y) 	*/
	void update_terrain_mesh(float t);

	void displayTerrain(environmentType const& environment);
	void create_empty_terrain();
	int logger = 0;
};