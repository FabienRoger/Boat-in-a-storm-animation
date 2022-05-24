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

Rock::Rock(){}

Rock::Rock(vec3 p, float r): position(p), radius(r){}

void Rock::initialize() {
    // Fonction d'initialisation
    rockMesh = mesh_primitive_sphere(radius);
    rockMeshDrawable.initialize(rockMesh, "rock");
    auto shader = opengl_load_shader("shaders/ocean/vert.glsl", "shaders/ocean/frag.glsl");
    rockMeshDrawable.shader = shader;
    rockMeshDrawable.shading.phong.specular = 0.2f;
    rockMeshDrawable.shading.color = {0.5f, 0.5f, 0.5f};
    rockMeshDrawable.transform.translation = position;

    addPerlinNoise(0.5);
    cut();
    addPerlinNoise(0.2);
}

void Rock::addPerlinNoise(float amplitude) {
    // Modifie terrain_mesh en fonction du temps et applique ce nouveau mesh au drawable
    for (auto& position : rockMesh.position) {
        float x = position.x;
        float y = position.y;
        float z = position.z;
        float size = 0.6f;
        vec3 delta = amplitude * position * noise_perlin({ size * x, size * y, size * z }, 3, 0.02f, 5.0f);
        position += delta;
    }

    rockMesh.compute_normal();                         // Ne pas oublier les normales du mesh évoluent - PI
    rockMeshDrawable.update_position(rockMesh.position);  // Mise à jour des positions - PI
    rockMeshDrawable.update_normal(rockMesh.normal);      // et des normales. - PI
}

void Rock::cut() {
    vec3 cutDirection = normalize(randVec3() - vec3(0.5,0.5,0.5));
    float cutLevel = 0;
    for (auto& mposition : rockMesh.position) {
        //vec3 deltap = mposition - position;
        float normDepth = dot(cutDirection, normalize(mposition));
        //std::cout << mposition << " ; " << deltap << " ; " << (normDepth > cutLevel) << std::endl;
        if (normDepth > cutLevel) {
            float depth = dot(cutDirection, mposition);
            vec3 projected = depth * cutDirection;
            mposition -= projected;
        }
    }
    rockMesh.compute_normal();                         // Ne pas oublier les normales du mesh évoluent - PI
    rockMeshDrawable.update_position(rockMesh.position);  // Mise à jour des positions - PI
    rockMeshDrawable.update_normal(rockMesh.normal);      // et des normales. - PI
}

void Rock::draw(StormEnvironment const& environment) {
    cgp::draw(rockMeshDrawable, environment);
}