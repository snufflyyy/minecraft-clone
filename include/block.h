#pragma once

#include <stdbool.h>

typedef enum Block_Type {
    AIR,
    GRASS,
    DIRT,
    STONE,
    COBBLESTONE
} Block_Type;

typedef struct Block {
    Block_Type type;

    int top_texture_index;
    int sides_texture_index;
    int bottom_texture_index;
    
    bool is_top_face_visible;
    bool is_bottom_face_visible;
    bool is_left_face_visible;
    bool is_right_face_visible;
    bool is_forward_face_visible;
    bool is_backward_face_visible;
} Block;

Block block_create(Block_Type block_type);