#version 330 core
out vec4 fragment_color;

in vec2 texture_coord;
in float light_amount;

uniform sampler2D block_texture;

void main() {
    fragment_color = texture(block_texture, texture_coord) * light_amount;
} 