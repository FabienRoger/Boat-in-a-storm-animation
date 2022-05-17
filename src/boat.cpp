
#include "boat.hpp"
#include "cgp/cgp.hpp"
#include "utils.hpp"

#include <vector>
#include<iostream>

using namespace cgp;
using namespace std;

void Boat::initialize()
{
	mesh boatMesh = mesh_load_file_obj("assets/boat.obj");
	boat_mesh.initialize(boatMesh, "boat mesh");
	boat_mesh.transform.scaling = .08f;

	generateStartSail();
	sail_mesh_drawable.initialize(sail_mesh, "sail");
	sail_mesh_drawable.shading.color = { 1.0f,1.0f,1.0f };
	sail_mesh_drawable.shading.phong.specular = 0.3f;
}

void Boat::draw(cgp::scene_environment_basic_camera_spherical_coords& env)
{
	boat_mesh.transform.translation = { position.x, position.y,position.z };
	cgp::draw(boat_mesh, env);
	cgp::draw(sail_mesh_drawable, env);
	update_sail();
}

void Boat::update(Terrain& terrain){
	float desiredZ = heightAboveWater + terrain.evaluate_terrain_height(position.x, position.y, terrain.timer.t);
	speed += vec3(0, 0, std::max((desiredZ - position.z) * kSpring, 0.f));
	speed += vec3(0, 0, - g);
	speed *= 1 - friction;
	position += speed * dt;

	update_sail();
}

void Boat::generateStartSail() {
	int nbPoints = 0;
	sailPositions.resize(nbVertical);
	sailSpeeds.resize(nbVertical);
	for (int i = 0; i < nbVertical; i++)
	{
		sailPositions[i] = vector<vec3>(nbHorizontal - i); // Triangular sail
		sailSpeeds[i] = vector<vec3>(nbHorizontal - i);
		for (int j = 0; j < nbHorizontal - i; j++)
		{
			vec3 verticalPos = i * sailHighPos / (nbVertical - 1);
			vec3 horizontalPos = j * sailEndPos / (nbHorizontal - 1);
			sailPositions[i][j] = sailLowPos + verticalPos + horizontalPos;
			nbPoints++;
		}
	}

	sail_mesh.position.resize(2*nbPoints);
	int point = 0;
	for (int i = 0; i < nbVertical; i++)
	{
		for (int j = 0; j < nbHorizontal - i; j++)
		{
			// double points to have different normals for both
			sail_mesh.position[2*point] = sailPositions[i][j]; // point "on one side"
			sail_mesh.position[2*point+1] = sailPositions[i][j]; // point "on the other"
			point++;
		}
	}

	//Cr�ation des triangles:
	point = -1;
	for (int i = 0; i < nbVertical; i++)
	{
		for (int j = 0; j < nbHorizontal - i; j++)
		{
			point++;
			if (i == 0)continue;
			if (j == nbHorizontal - i - 1) {
				int u = point;
				int v = point - (nbHorizontal - i);
				int w = point - (nbHorizontal - i) - 1;
				sail_mesh.connectivity.push_back(
					{ 2 * u, 2 * v, 2 * w }
				);
				sail_mesh.connectivity.push_back(
					{ 2 * u + 1, 2 * v + 1, 2 * w + 1}
				);
			}
			if (j > 0) {
				int u = point;
				int v = point - (nbHorizontal - i) - 1;
				int w = point - (nbHorizontal - i) - 2;
				sail_mesh.connectivity.push_back(
					{ 2 * u, 2 * v, 2 * w }
				);
				sail_mesh.connectivity.push_back(
					{ 2 * u + 1, 2 * v + 1, 2 * w + 1 }
				);

				u = point - 1;
				v = point;
				w = point - (nbHorizontal - i) - 2;

				sail_mesh.connectivity.push_back(
					{ 2 * u, 2 * v, 2 * w }
				);
				sail_mesh.connectivity.push_back(
					{ 2 * u + 1, 2 * v + 1, 2 * w + 1 }
				);
			}
		}
	}


	sail_mesh.fill_empty_field();
}

void Boat::update_sail()
{

	for (int i = 0; i < nbVertical; i++)
	{
		for (int j = 0; j < nbHorizontal - i; j++)
		{
			sailSpeeds[i][j] += wind;
			vec3 idealPosition = vec3(0, 0, 0);

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
			// mat
			else if (i == 0 && i < nbVertical - 1 && j > 0 && j < nbHorizontal - i - 1) {
				idealPosition += sailPositions[i][j - 1];
				idealPosition += sailPositions[i][j + 1];
				idealPosition /= 2;
			}
			// bas
			else if (i > 0 && i < nbVertical - 1 && j == 0 && j < nbHorizontal - i - 1) {
				idealPosition += sailPositions[i - 1][j];
				idealPosition += sailPositions[i + 1][j];
				idealPosition /= 2;
			}

			sailSpeeds[i][j] += kSailSpring * (idealPosition - sailPositions[i][j]);

			sailSpeeds[i][j] *= 1 - sailFriction;
		}
	}

	for (int i = 0; i < nbVertical; i++)
	{
		for (int j = 0; j < nbHorizontal - i; j++)
		{
			sailPositions[i][j] += dt * sailSpeeds[i][j];
		}
	}

	sailPositions[0][0] = sailLowPos;
	sailPositions[nbVertical - 1][0] = sailHighPos + sailLowPos;
	sailPositions[0][nbHorizontal - 1] = sailEndPos + sailLowPos;

	int point = 0;
	for (int i = 0; i < nbVertical; i++)
	{
		for (int j = 0; j < nbHorizontal - i; j++)
		{
			sail_mesh.position[2 * point] = sailPositions[i][j] + position;
			sail_mesh.position[2*point+1] = sailPositions[i][j] + position;
			point++;
		}
	}

	sail_mesh.compute_normal();//Ne pas oublier les normales du mesh évoluent - PI
	sail_mesh_drawable.update_position(sail_mesh.position);//Mise à jour des positions - PI
	sail_mesh_drawable.update_normal(sail_mesh.normal);//et des normales. - PI
}
