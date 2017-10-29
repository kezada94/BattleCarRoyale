#version 410

in vec3 vp;

uniform mat4 P, V;
out vec3 texcoords;

void main () {
	texcoords = vp;
	vec3 cam_pos_wor = (inverse(V) * vec4 (0.0, 0.0, 0.0, 1.0)).xyz; //saco la posicion de la camara para q el skybox siempre este a la misma distancia del ojo
	gl_Position = P * V * vec4 (cam_pos_wor+vp*50, 1.0);
}
