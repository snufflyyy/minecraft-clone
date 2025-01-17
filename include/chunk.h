#pragma once

#define CHUNK_SIZE 16

#include "block.h"
#include "vertex.h"
#include "math/matrix4f.h"

typedef struct Chunk {
    Block blocks[CHUNK_SIZE * CHUNK_SIZE * CHUNK_SIZE];

    Vertex* vertices;
    int number_of_vertices;
    unsigned int* indices;
    int number_of_indices;
    
    unsigned int VAO, VBO, EBO;
} Chunk;

Chunk chunk_create();
void chunk_update(Chunk* chunk, Chunk* neighbors[6]);
void chunk_generate_mesh(Chunk* chunk);
void chunk_draw(Chunk* chunk);
void chunk_destroy(Chunk* chunk);