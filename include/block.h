#pragma once

#include <stdbool.h>

typedef enum Block_Type {
    AIR,
    GRASS
} Block_Type;

typedef struct Block {
    Block_Type type;
    
    bool is_top_face_visible;
    bool is_bottom_face_visible;
    bool is_left_face_visible;
    bool is_right_face_visible;
    bool is_forward_face_visible;
    bool is_backward_face_visible;
} Block;

Block block_create(Block_Type block_type);