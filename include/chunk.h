#pragma once

#define CHUNK_SIZE 16

#include "block.h"

typedef struct Chunk {
    Block blocks[CHUNK_SIZE][CHUNK_SIZE][CHUNK_SIZE];
    unsigned int VAO, VBO, EBO;
} Chunk;

Chunk chunk_create();