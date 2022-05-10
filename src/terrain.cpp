// Nathan - PI = pas d'impact significatif sur les performances
// Gros pb de perf: pour avoir un oc�an pas trop d�gueu il faut 200 samples mais fps vers 30

#include "terrain.hpp"
#include "cgp/cgp.hpp"


using namespace cgp;

void Terrain::initialize() {
    //Fonction d'initialisation
    create_empty_terrain(); 
    mesh_drawable.initialize(terrain_mesh, "terrain");
    mesh_drawable.shading.color = { 0.1f,0.1f,0.8f };//Couleur de l'eau
    mesh_drawable.shading.phong.specular = 0.2f; //� d�finir pour l'eau
}

float Terrain::evaluate_terrain_height(float x, float y, float t)
{
    //Calcule la hauteur d'un point de coordonn�e xy au temps t
    float h_0 = 2.0f;
    float z = h_0 * std::cos(0.01*(x*x + y*y + 50*t));// PI
    return z;
}

void Terrain::displayTerrain(environmentType const& environment) {
    timer.update();
    update_terrain_mesh(timer.t);
    draw(mesh_drawable, environment);
}


void Terrain::update_terrain_mesh(float t)
{
    //Modifie terrain_mesh en fonction du temps et applique ce nouveau mesh au drawable
    int N = N_terrain_samples;
    for(int ku=0; ku<N; ++ku)
    {
        for(int kv=0; kv<N; ++kv)
        {
            float x = (ku / (N - 1.0f) - 0.5f) * terrain_length;
            float y = (kv / (N - 1.0f) - 0.5f) * terrain_length;

            float z = evaluate_terrain_height(x,y, t); // PI

            terrain_mesh.position[kv + N * ku].z = z;
        }
    }
    terrain_mesh.compute_normal();//Ne pas oublier les normales du mesh �voluent - PI
    mesh_drawable.update_position(terrain_mesh.position);//Mise � jour des positions - PI
    mesh_drawable.update_normal(terrain_mesh.normal);//et des normales. - PI
}

void Terrain::create_empty_terrain()
{
    //Cr�e un mesh de taille N*N en initialisant les points � une hauteur de 0;
    int N = N_terrain_samples;
    terrain_mesh.position.resize(N * N);

    for (int ku = 0; ku < N; ++ku)
    {
        for (int kv = 0; kv < N; ++kv)
        {
            float u = ku / (N - 1.0f);
            float v = kv / (N - 1.0f);

            float x = (u - 0.5f) * terrain_length;
            float y = (v - 0.5f) * terrain_length;

            terrain_mesh.position[kv + N * ku] = { x,y,0.0f};
        }
    }

    //Cr�ation des triangles:
    for (int ku = 0; ku < N - 1; ++ku)
    {
        for (int kv = 0; kv < N - 1; ++kv)
        {
            unsigned int idx = kv + N * ku; 

            uint3 triangle_1 = { idx, idx + 1 + N, idx + 1 };
            uint3 triangle_2 = { idx, idx + N, idx + 1 + N };

            terrain_mesh.connectivity.push_back(triangle_1);
            terrain_mesh.connectivity.push_back(triangle_2);
        }
    }

    //Le reste est rempli par des valeurs par d�faut:
    terrain_mesh.fill_empty_field();

}

