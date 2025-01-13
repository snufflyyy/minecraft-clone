#pragma once

#include "block.h"

typedef struct Chunk {
    Block blocks[16][16][16];
    unsigned int VAO, VBO, EBO;
} Chunk;

Chunk chunk_create();