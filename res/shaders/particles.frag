#version 410

out vec4 frag_colour;
uniform sampler2D tex; // optional. enable point-sprite coords to use
vec4 particle_colour = vec4 (1, 1, 0, 1);

void main(){
    vec4 texel = texture (tex, gl_PointCoord);
    frag_colour = texel;
}