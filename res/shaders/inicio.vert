#version 130
    in vec3 position;
    in vec3 color;
    in vec2 texCoord;
    out vec3 ourColor;
    out vec2 TexCoord;

    void main(){
        gl_Position = vec4(position, 1.0f);
        ourColor = color;
        // We swap the y-axis by substracing our coordinates from 1. This is done because most images have the top y-axis inversed with OpenGL's top y-axis.
        // TexCoord = texCoord;
        TexCoord = vec2(texCoord.x, 1.0 - texCoord.y);
    }