#version 330 core
in vec2 TexCoords;
out vec4 color;

uniform sampler2D image;
uniform vec2 uvScale;
uniform vec2 uvOffset;

void main(){
    vec2 uv = TexCoords * uvScale + uvOffset;
    color = texture(image, uv);
}
