#version 330 core  // OpenGL 3.3 shader

// Fragment shader - this code is executed for every pixel/fragment that belongs to a displayed shape

// Inputs coming from the vertex shader
flat in vec3 fragment_position; // position in the world space, flat because of the low poly effect
flat in vec3 fragment_normal;   // normal in the world space, flat because of the low poly effect
in vec3 fragment_color;    // current color on the fragment
in vec2 fragment_uv;       // current uv-texture on the fragment
// Output of the fragment shader - output color
layout(location=0) out vec4 FragColor;

// Texture image
uniform sampler2D image_texture;

// Uniform values - must be send from the C++ code
uniform vec3 light; // position of the light
uniform vec3 lightDirection; // direction of the light
uniform float lightIntensity; // lightning intensity
uniform vec3 lightningDirection; // direction of the lightning

// Shape color
uniform vec3 color;   // Uniform color of the object
uniform float alpha;  // alpha coefficient

// Phong shading
uniform float Ka;     // Ambient coefficient
uniform float Kd;     // Diffuse coefficient
uniform float Ks;     // Specular coefficient
uniform float specular_exp; // Specular exponent

uniform mat4 view;       // View matrix (rigid transform) of the camera - to compute the camera position

const float minFogDist = 10;
const float maxDist = 50;
const vec3 backgroundColor = vec3(0.2,0.2,0.2);
const vec3 white = vec3(1,1,1);
const float PI = 3.1415926535;

float getFog(float distanceToCam){
	if(distanceToCam > maxDist)return 1;
	if(distanceToCam < minFogDist)return 0;
	float normalizedDist = (distanceToCam - minFogDist) / (maxDist - minFogDist);
	// return sin(normalizedDist * PI / 2);
	return sin(normalizedDist * PI / 2);
}

void main()
{

	// Compute the position of the center of the camera
	mat3 O = transpose(mat3(view));                   // get the orientation matrix
	vec3 last_col = vec3(view*vec4(0.0, 0.0, 0.0, 1.0)); // get the last column
	vec3 camera_position = -O*last_col;

	// Re-normalize the normals (interpolated on the triangle)
	vec3 N = normalize(fragment_normal);

	// Inverse the normal if it is viewed from its back
	//  (note: doesn't work on Mac)
	if (gl_FrontFacing == false) {  
		N = -N;
	}

	// Phong coefficient (diffuse, specular)
	// ************************* //

	// Unit direction toward the light
	vec3 L = normalize(light-fragment_position);

	// Diffuse coefficient
	float diffuse = max(dot(N,L),0.0);

	// In the light cone coefficient
	float d = dot(lightDirection, -L) + 0.2;
	float inCone = d > 0 ? 4*d*d : 0;
	float lightPower = max(inCone, 0.4);

	// Specular coefficient
	float specular = 0.0;
	if(diffuse>0.0){
		vec3 R = reflect(-L,N); // symetric of light-direction with respect to the normal
		vec3 V = normalize(camera_position-fragment_position);
		specular = pow( max(dot(R,V),0.0), specular_exp );
	}

	diffuse *= lightPower;
	specular *= lightPower;

	// Texture
	// ************************* //
	
	// Current uv coordinates
	// by default inverse the v direction (avoids common image upside-down)
	vec2 uv_image = vec2(fragment_uv.x, 1.0-fragment_uv.y);

	// Get the current texture color
	vec4 color_image_texture = texture(image_texture, uv_image);


	// Compute Shading
	// ************************* //

	// Compute the base color of the object based on: vertex color, uniform color, and texture
	vec3 color_object  = fragment_color * color * color_image_texture.rgb;

	// Compute the final shaded color using Phong model
	vec3 color_shading = (Ka + Kd * diffuse) * color_object + Ks * specular * vec3(1.0, 1.0, 1.0);
	
	// Compute color with fog
	float distToCam = length(camera_position-fragment_position);
	float fog = getFog(distToCam);


	float globalLightIntensity = min(1, lightIntensity * 0.2f + lightIntensity * dot(lightningDirection, N) * 2.0f);

	vec3 color_shading_illuminated = (1 - globalLightIntensity) * color_shading + globalLightIntensity * white;
	
	vec3 actualBackgroundColor = (1 - lightIntensity) * backgroundColor + lightIntensity * white;

	vec3 color_fog = actualBackgroundColor * fog + (1 - fog) * color_shading_illuminated;

	// Output color, with the alpha component
	FragColor = vec4(color_fog, alpha * color_image_texture.a);
}