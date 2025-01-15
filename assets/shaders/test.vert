#version 330 core
layout (location = 0) in vec3 a_position;
layout (location = 1) in vec3 a_normal;
layout (location = 2) in vec2 a_texture_coord;

out vec2 texture_coord;
out float light_amount;

uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;

void main() {
    gl_Position = projection * view * model * vec4(a_position, 1.0);
    texture_coord = a_texture_coord;

    if (a_normal.z == 1.0) {
        light_amount = 0.86;
    } else if (a_normal.x == 1.0) {
        light_amount = 0.80;
    } else {
        light_amount = 1.0;
    }
}