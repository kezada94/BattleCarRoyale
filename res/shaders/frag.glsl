#version 410 //130 linux - 410 mac

in vec3 normalEye;
in vec3 normal;
in vec2 st;
in vec3 positionEye;

in vec3 view_dir_tan;
in vec3 light_dir_tan;

out vec4 frag_colour;

uniform sampler2D basic_texture;
uniform sampler2D normal_map;
uniform mat4 view;

void main() {

	vec3 normal_tan = texture (normal_map, st).rgb;
	normal_tan = normalize (normal_tan * 2.0 - 1.0);

	// Diffuse factor done in eye space
	vec3 lightPositionEye = vec3(view * vec4(30, 60, 0, 1));
	vec3 distanceToLightEye = lightPositionEye - positionEye;
	vec3 directionToLightEye = normalize(distanceToLightEye);
	float diffuse = clamp(dot(normalEye, directionToLightEye), 0, 1);

	// specular light equation done in tangent space,
	// We used Phong method because Blinn was not so good-looking
	vec3 direction_to_light_tan = normalize(-light_dir_tan);
	vec3 reflection_tan = reflect (normalize (light_dir_tan), normal_tan);
	float dot_prod_specular = dot (reflection_tan, normalize (view_dir_tan));
	dot_prod_specular = max (dot_prod_specular, 0.0);
	float specular_factor = pow (dot_prod_specular, 20.0);

	vec4 texel = texture (basic_texture, st);

	frag_colour = texel * vec4(0.1, 0.1, 0.1, 1) + (texel * diffuse) + (specular_factor);
}
