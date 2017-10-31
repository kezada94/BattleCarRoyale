#version 130
//layout (location = 0) in vec3 position; //MAC

in vec3 position;  //LINUX

uniform mat4 projection;
uniform mat4 view;

void main()
{
    gl_Position = projection * view * vec4(position, 1.0f);
}