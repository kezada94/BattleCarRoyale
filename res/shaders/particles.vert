#version 130
//layout (location = 0) in vec3 v_i;                  //
//layout (location = 1) in double start_time;         // MAC
//layout (location = 2) in vec3 origin;               //

in vec3 v_i;                  //
//in double start_time;         // LINUX
in vec3 origin;               //


uniform mat4 V, P;
//uniform double elapsed_system_time;
void main(){
//    double t = elapsed_system_time;
//    dvec3 p = origin + normalize(v_i) * t * 420.f; //speed

    gl_Position = vec4(0,0,0,1);
//    double size = clamp(5.0/(elapsed_system_time*3), 0, 10);
//    gl_PointSize = float(size);
}