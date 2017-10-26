#version 410

layout(location = 0) in vec3 vertex_position; //
layout(location = 1) in vec3 vertex_normal;   //* PARA MAC
layout(location = 2) in vec2 texture_coord;   //

//in vec3 vertex_position;    //
//in vec3 vertex_normal;      //* PARA LINUX
//in vec2 texture_coord;      //

uniform mat4 view, proj, model;

out vec3 normalEye;
out vec2 st;
out vec3 positionEye;

void main() {

	st = texture_coord;
	normalEye = normalize(vec3(view * model * vec4(vertex_normal, 0.0)));
	positionEye = vec3(view * model * vec4(vertex_position, 1.0));

	gl_Position = proj * view * model * vec4 (vertex_position, 1.0);
}
