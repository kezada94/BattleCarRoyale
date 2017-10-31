#version 130
//layout (location = 0) in vec3 v_i;                  //
//layout (location = 1) in float start_time;         // MAC
//layout (location = 2) in vec3 origin;               //

in vec3 v_i;            //
in vec3 origin;         //

uniform mat4 V, P;
uniform float elapsed_system_time;
void main(){
    float t = elapsed_system_time;
    vec3 p =  origin + normalize(v_i) * t * 420.f; //speed

    gl_Position = P * V * vec4 (p, 1.0);
    float size = clamp(5.0/(elapsed_system_time*3), 0, 10);
    gl_PointSize = float(size);
}