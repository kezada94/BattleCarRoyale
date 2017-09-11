#version 410 //130 linux - 410 mac

in vec3 normal;
in vec2 st;
out vec4 frag_colour;

void main() {
	frag_colour = vec4 (normal, 1.0);
}
