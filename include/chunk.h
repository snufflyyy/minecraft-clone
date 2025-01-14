#pragma once

#define CHUNK_SIZE 16

#include "block.h"
#include "vertex.h"
#include "math/matrix4f.h"

typedef struct Chunk {
    Block blocks[CHUNK_SIZE][CHUNK_SIZE][CHUNK_SIZE];

    Vertex* vertices;
    int number_of_vertices;
    unsigned int* indices;
    int number_of_indices;
    
    unsigned int VAO, VBO, EBO;
    Matrix4f model;
} Chunk;

Chunk chunk_create();
void chunk_draw(Chunk* chunk);