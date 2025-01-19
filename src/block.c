#include "block.h"

Block block_create(Block_Type block_type) {
    Block block = {0};

    block.type = block_type;

    switch (block.type) {
        case AIR:
            break;
        case GRASS:
            block.top_texture_index = 0;
            block.sides_texture_index = 1;
            block.bottom_texture_index = 2;
            break;
        case DIRT:
            block.top_texture_index = 2;
            block.sides_texture_index = 2;
            block.bottom_texture_index = 2;
            break;
        case STONE:
            block.top_texture_index = 3;
            block.sides_texture_index = 3;
            block.bottom_texture_index = 3;
            break;
        case COBBLESTONE:
            block.top_texture_index = 4;
            block.sides_texture_index = 4;
            block.bottom_texture_index = 4;
            break;
        default:
            block.top_texture_index = 256;
            block.sides_texture_index = 256;
            block.bottom_texture_index = 256;
            break;
    }

    return block;
}