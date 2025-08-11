#version 420 core
out vec4 color;
in vec2 texcoords;
uniform sampler2D texture;
void main() {
    color = texture2D(texture, texcoords);
}