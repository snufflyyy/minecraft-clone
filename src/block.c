#include "block.h"

Block block_create(Block_Type block_type) {
    Block block = {0};

    block.type = block_type;

    return block;
}