#version 420 core
layout(location = 0) in vec3 position;
layout(location = 1) in vec2 coords;
out vec2 texcoords;
uniform mat4 projection;
uniform mat4 view;
void main() {
    gl_Position = projection * view * vec4(position, 1.0);
    texcoords = coords;
}