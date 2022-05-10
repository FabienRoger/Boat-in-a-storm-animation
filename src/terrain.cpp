// Nathan - PI = pas d'impact significatif sur les performances
// Gros pb de perf: pour avoir un océan pas trop dégueu il faut 200 samples mais fps vers 30

//Passer par un shader ? tester https://github.com/drohmer/CGP/tree/main/scenes/examples/03_shader_effects/01_shader_deformation
// et si plus éfficace, implémenter

//Beaucoup de triangles quand même
//si on se sert de la forme juste pour la houle, on peut, à l'aide de la dérivée, choisir de détailler certains carrés plus que 
//d'autres. Il faut que le nombre de triangle reste constant (par ex 10 000 triangles de base et on en coupe en deux 2000.

//autre idée (compatible mais ça devient hardcore) c'est de prendre une surface au nombre de triangles constant mais la plaquer sous la camera en temps réel.


#include "terrain.hpp"
#include "cgp/cgp.hpp"


using namespace cgp;

void Terrain::initialize() {
    //Fonction d'initialisation
    create_empty_terrain(); 
    mesh_drawable.initialize(terrain_mesh, "terrain");
    //mesh_drawable.shading.color = { 0.1f,0.1f,0.8f };//Couleur de l'eau
    mesh_drawable.shading.phong.specular = 0.2f; //à définir pour l'eau
    GLuint const texture_image_id = opengl_load_texture_image("assets/water_texture.jpg",
        GL_REPEAT,
        GL_REPEAT);
    mesh_drawable.texture = texture_image_id;
}

float Terrain::evaluate_terrain_height(float x, float y, float t)
{
    //Calcule la hauteur d'un point de coordonnée xy au temps t
    float h_0 = 5.0f;
    float omega = 0.1f;
    float A = 0.2f;
    float B = 0.5f;

    float z = h_0 - h_0 * std::max(std::abs(std::cos(x * omega - A*t)), 0.05f) + B * noise_perlin({ 0.3*x,0.3*y,0.5*t }, 3, 0.02f, 5.0f); //x octave persistency, frequency gain
    return z;
}

void Terrain::displayTerrain(environmentType const& environment) {
    timer.update();
    update_terrain_mesh(timer.t);
    draw(mesh_drawable, environment);// fonction qui consomme à mort la ressource
    //draw_wireframe(mesh_drawable, environment);
}


void Terrain::update_terrain_mesh(float t)
{
    //Modifie terrain_mesh en fonction du temps et applique ce nouveau mesh au drawable
    int N = N_terrain_samples;
    for(int ku=0; ku<N; ++ku)
    {
        for(int kv=0; kv<N; ++kv)
        {
            int k = kv + N * ku;
            float x = (ku / (N - 1.0f) - 0.5f) * terrain_length;
            float y = (kv / (N - 1.0f) - 0.5f) * terrain_length;

            float z = evaluate_terrain_height(x,y, t); // PI

            terrain_mesh.position[k].z = z;
        }
    }
    terrain_mesh.compute_normal();//Ne pas oublier les normales du mesh évoluent - PI
    mesh_drawable.update_position(terrain_mesh.position);//Mise à jour des positions - PI
    mesh_drawable.update_normal(terrain_mesh.normal);//et des normales. - PI
    //mesh_drawable.update_color(terrain_mesh.color);
}

void Terrain::create_empty_terrain()
{
    //Crée un mesh de taille N*N en initialisant les points à une hauteur de 0;
    //Passer par mesh_primitive_grid consomme un petit plus de ressource lors de l'update
    int N = N_terrain_samples;
    terrain_mesh.position.resize(N * N);
    terrain_mesh.uv.resize(N * N);

    for (int ku = 0; ku < N; ++ku)
    {
        for (int kv = 0; kv < N; ++kv)
        {
            float u = ku / (N - 1.0f);
            float v = kv / (N - 1.0f);

            float x = (u - 0.5f) * terrain_length;
            float y = (v - 0.5f) * terrain_length;

            terrain_mesh.position[kv + N * ku] = { x,y,0.0f};
            terrain_mesh.uv[kv + N * ku] = { 20*u,20*v };
        }
    }

    //Création des triangles:
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

    //Le reste est rempli par des valeurs par défaut:
    terrain_mesh.fill_empty_field();

}

