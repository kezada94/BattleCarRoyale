#version 130

// Ouput data
//layout(location = 0) out float fragmentdepth;
out float fragmentdepth;

void main(){
    // Not really needed, OpenGL does it anyway
    fragmentdepth = gl_FragCoord.z;
}