#version 130 //130 linux - 410 mac

in vec3 normalEye;
in vec2 st;
in vec3 positionEye;

out vec4 frag_colour;

uniform sampler2D basic_texture;
uniform mat4 view;

void main() {
	vec3 lightPositionEye = vec3(view * vec4(30, 60, 0, 1));
	vec3 distanceToLightEye = lightPositionEye - positionEye;
	vec3 directionToLightEye = normalize(distanceToLightEye);
	float diffuse = clamp(dot(normalEye, directionToLightEye), 0, 1);

	vec3 surfacetoViewer = normalize(positionEye);
	vec3 h = normalize(directionToLightEye + surfacetoViewer);
	float specular = pow(clamp(dot(h, normalEye), 0, 1), 20);
	vec4 texel = texture (basic_texture, st);

	float distanceSquare = distance(lightPositionEye, positionEye);
	frag_colour = texel * vec4(0.1, 0.1, 0.1, 1) + (texel * diffuse) + (specular);
}
