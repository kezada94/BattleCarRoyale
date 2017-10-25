#version 130 //130 linux - 410 mac

in vec3 normal;
in vec2 st;
out vec4 frag_colour;

uniform sampler2D basic_texture;

void main() {
	frag_colour = texture (basic_texture, st);
}
