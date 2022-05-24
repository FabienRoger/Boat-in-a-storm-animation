// Nathan - PI = pas d'impact significatif sur les performances
// Gros pb de perf: pour avoir un oc�an pas trop d�gueu il faut 200 samples mais fps vers 30

// Passer par un shader ? tester https://github.com/drohmer/CGP/tree/main/scenes/examples/03_shader_effects/01_shader_deformation
//  et si plus �fficace, impl�menter

// Beaucoup de triangles quand m�me
// si on se sert de la forme juste pour la houle, on peut, � l'aide de la d�riv�e, choisir de d�tailler certains carr�s plus que
// d'autres. Il faut que le nombre de triangle reste constant (par ex 10 000 triangles de base et on en coupe en deux 2000.

// autre id�e (compatible mais �a devient hardcore) c'est de prendre une surface au nombre de triangles constant mais la plaquer sous la camera en temps r�el.

#include "rock.hpp"

#include "cgp/cgp.hpp"
#include "utils.hpp"

using namespace cgp;

void Rock::initialize() {
    // Fonction d'initialisation
    rockMesh = mesh_primitive_sphere(radius);
    rockMeshDrawable.initialize(rockMesh, "rock");
    auto shader = opengl_load_shader("shaders/ocean/vert.glsl", "shaders/ocean/frag.glsl");
    rockMeshDrawable.shader = shader;
    rockMeshDrawable.shading.phong.specular = 0.2f;
    rockMeshDrawable.shading.color = {0.5f, 0.5f, 0.5f};
    rockMeshDrawable.transform.translation = position;
    improveDetails();
}

void Rock::improveDetails() {
    // Modifie terrain_mesh en fonction du temps et applique ce nouveau mesh au drawable
    /*for (auto& position : rockMesh.position) {
        float x = position.x;
        float y = position.y;
        float z = position.z;
        vec3 centerToPos = 
        vec3 delta = noise_perlin({ 0.3 * x, 0.3 * y, 0.5 * z }, 3, 0.02f, 5.0f);
        position += delta;
    }*/

    rockMesh.compute_normal();                         // Ne pas oublier les normales du mesh �voluent - PI
    rockMeshDrawable.update_position(rockMesh.position);  // Mise � jour des positions - PI
    rockMeshDrawable.update_normal(rockMesh.normal);      // et des normales. - PI
    // mesh_drawable.update_color(terrain_mesh.color);
}

void Rock::draw(StormEnvironment const& environment) {
    cgp::draw(rockMeshDrawable, environment);
}