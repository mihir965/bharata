#version 330

layout (location=0) in vec2 aPos;
layout (location=1) in vec2 aUV;

out vec2 TexCoords;

uniform mat4 model;
uniform mat4 projection;


void main(){
    TexCoords = aUV;
    gl_Position = projection * model * vec4(aPos, 0.0, 1.0);
}
