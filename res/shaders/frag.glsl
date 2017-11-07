#version 130 //130 linux - 410 mac

in vec3 normalEye;
in vec3 normal;
in vec2 st;
in vec3 positionEye;

in vec3 EyeDirection_tangent;
in vec3 LightDirection_tangent;

out vec4 frag_colour;

uniform sampler2D basic_texture;
uniform sampler2D normal_map;
uniform mat4 view;

void main() {

	vec3 normal_tan = texture (normal_map, st).rgb;
	
	normal_tan = normalize (normal_tan * 2.0 - 1.0);
	


	// Diffuse factor done in eye space
	vec3 lightPositionEye = vec3(view * vec4(30, 100, 0, 1));
	vec3 distanceToLightEye = lightPositionEye - positionEye;
	vec3 directionToLightEye = normalize(distanceToLightEye);
	float diffuse = clamp(dot(normalEye, directionToLightEye), 0, 1);

	// Eye vector (towards the camera)
	vec3 E = normalize(EyeDirection_tangent);
	vec3 H = normalize(EyeDirection_tangent+normalize(LightDirection_tangent));
	// Direction in which the triangle reflects the light
	vec3 R = reflect(-normalize(LightDirection_tangent), normal_tan);
	float cosAlpha = clamp(dot(E, R), 0, 1);



	vec4 texel = texture (basic_texture, st);

	frag_colour = texel * vec4(0.1, 0.1, 0.1, 1) + (texel * diffuse) + (pow(cosAlpha, 20));
}
