#pragma once

typedef enum Block_Type {
    AIR,
    GRASS
} Block_Type;

typedef struct Block {
    Block_Type block_type;
    
    bool is_top_face_visable;
    bool is_bottom_face_visable;
    bool is_left_face_visable;
    bool is_right_face_visable;
    bool is_forward_face_visable;
    bool is_backward_face_visable;
} Block;