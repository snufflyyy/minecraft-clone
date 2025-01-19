#version 330 core
out vec4 fragment_color;

in vec2 texture_coord;
in float texture_index;
in float light_amount;

uniform sampler2D texture_atlas;

void main() {
    int grid_size = 16;

    float x = mod(texture_index, grid_size); 
    float y = floor(texture_index / grid_size);

    vec2 sub_texture_size = vec2(1.0 / grid_size, 1.0 / grid_size);
    vec2 offset = vec2(x, y) * sub_texture_size;
    vec2 new_texture_coord = (texture_coord * sub_texture_size) + offset;

    fragment_color = texture(texture_atlas, new_texture_coord) * light_amount;
}