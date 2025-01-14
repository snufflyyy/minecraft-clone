#include "chunk.h"

#include <stdlib.h>

#include <glad/glad.h>

// TEMP
static const float test_vertices[] = {
    // front face
    -1.0f, -1.0f, -1.0f,  0.0f, 0.0f,  0.0f,  0.0f, -1.0f, // bottom left
     0.0f, -1.0f, -1.0f,  1.0f, 0.0f,  0.0f,  0.0f, -1.0f, // bottom right
     0.0f,  1.0f, -1.0f,  1.0f, 1.0f,  0.0f,  0.0f, -1.0f, // top right
    -1.0f,  1.0f, -1.0f,  0.0f, 1.0f,  0.0f,  0.0f, -1.0f, // top left

    // back face
    -1.0f, -1.0f,  1.0f,  0.0f, 0.0f,  0.0f,  0.0f,  1.0f, // bottom left
     1.0f, -1.0f,  1.0f,  1.0f, 0.0f,  0.0f,  0.0f,  1.0f, // bottom right
     1.0f,  1.0f,  1.0f,  1.0f, 1.0f,  0.0f,  0.0f,  1.0f, // top right
    -1.0f,  1.0f,  1.0f,  0.0f, 1.0f,  0.0f,  0.0f,  1.0f, // top left

    // left face
    -1.0f, -1.0f,  1.0f,  1.0f, 0.0f, -1.0f,  0.0f,  0.0f, // bottom right
    -1.0f, -1.0f, -1.0f,  0.0f, 0.0f, -1.0f,  0.0f,  0.0f, // bottom left
    -1.0f,  1.0f, -1.0f,  0.0f, 1.0f, -1.0f,  0.0f,  0.0f, // top left
    -1.0f,  1.0f,  1.0f,  1.0f, 1.0f, -1.0f,  0.0f,  0.0f, // top right

    // right face
     1.0f, -1.0f,  1.0f,  1.0f, 0.0f,  1.0f,  0.0f,  0.0f, // bottom right
     1.0f, -1.0f, -1.0f,  0.0f, 0.0f,  1.0f,  0.0f,  0.0f, // bottom left
     1.0f,  1.0f, -1.0f,  0.0f, 1.0f,  1.0f,  0.0f,  0.0f, // top left
     1.0f,  1.0f,  1.0f,  1.0f, 1.0f,  1.0f,  0.0f,  0.0f, // top right

    // bottom face
    -1.0f, -1.0f,  1.0f,  0.0f, 1.0f,  0.0f, -1.0f,  0.0f, // top left
     1.0f, -1.0f,  1.0f,  1.0f, 1.0f,  0.0f, -1.0f,  0.0f, // top right
     1.0f, -1.0f, -1.0f,  1.0f, 0.0f,  0.0f, -1.0f,  0.0f, // bottom right
    -1.0f, -1.0f, -1.0f,  0.0f, 0.0f,  0.0f, -1.0f,  0.0f, // bottom left

    // top face
    -1.0f,  1.0f,  1.0f,  0.0f, 1.0f,  0.0f,  1.0f,  0.0f, // top left
     1.0f,  1.0f,  1.0f,  1.0f, 1.0f,  0.0f,  1.0f,  0.0f, // top right
     1.0f,  1.0f, -1.0f,  1.0f, 0.0f,  0.0f,  1.0f,  0.0f, // bottom right
    -1.0f,  1.0f, -1.0f,  0.0f, 0.0f,  0.0f,  1.0f,  0.0f  // bottom left
};
static const unsigned int test_indices[] = {
    // front face
    0, 1, 2,
    0, 2, 3,

    // back face
    4, 5, 6,
    4, 6, 7,

    // left face
    8, 9, 10,
    8, 10, 11,

    // right face
    12, 13, 14,
    12, 14, 15,

    // bottom face
    16, 17, 18,
    16, 18, 19,

    // top face
    20, 21, 22,
    20, 22, 23
};

Chunk chunk_create() {
    Chunk chunk = {0};

    // temp until world generation is done
    // just setting all the blocks to grass for now
    for (int x = 0; x < CHUNK_SIZE; x++) {
        for (int y = 0; y < CHUNK_SIZE; y++) {
            for (int z = 0; z < CHUNK_SIZE; z++) {
                chunk.blocks[x][y][z].type = GRASS;
            }
        }
    }

    chunk.number_of_vertices = 0;
    chunk.number_of_indices = 0;

    for (int x = 0; x < CHUNK_SIZE; x++) {
        for (int y = 0; y < CHUNK_SIZE; y++) {
            for (int z = 0; z < CHUNK_SIZE; z++) {
                if (chunk.blocks[x][y][z].type == AIR) { continue; }

                chunk.blocks[x][y][z].is_left_face_visable     = (x == 0 || chunk.blocks[x - 1][y][z].type == AIR);
                chunk.blocks[x][y][z].is_right_face_visable    = (x == CHUNK_SIZE - 1 || chunk.blocks[x + 1][y][z].type == AIR);
                chunk.blocks[x][y][z].is_top_face_visable      = (y == 0 || chunk.blocks[x][y - 1][z].type == AIR);
                chunk.blocks[x][y][z].is_bottom_face_visable   = (y == CHUNK_SIZE - 1 || chunk.blocks[x][y + 1][z].type == AIR);
                chunk.blocks[x][y][z].is_forward_face_visable  = (z == 0 || chunk.blocks[x][y][z + 1].type == AIR);
                chunk.blocks[x][y][z].is_backward_face_visable = (z == CHUNK_SIZE - 1 || chunk.blocks[x][y][z - 1].type == AIR);

                // generate faces
                if (chunk.blocks[x][y][z].is_left_face_visable) {
                    chunk.vertices = realloc(chunk.vertices, (chunk.number_of_vertices + 4) * sizeof(Vertex));
                    chunk.indices = realloc(chunk.indices, (chunk.number_of_indices + 6) * sizeof(unsigned int));

                    // set vertices
                    Vertex v0 = {
                        .position = {x - 1.0f, y - 1.0f, z + 1.0f},
                        .normal = {-1.0f, 0.0f, 0.0f},
                        .texture_coord = {1.0f, 0.0f}
                    };
                    Vertex v1 = {
                        .position = {x - 1.0f, y - 1.0f, z - 1.0f},
                        .normal = {-1.0f, 0.0f, 0.0f},
                        .texture_coord = {0.0f, 0.0f}
                    };
                    Vertex v2 = {
                        .position = {x - 1.0f, y + 1.0f, z - 1.0f},
                        .normal = {-1.0f, 0.0f, 0.0f},
                        .texture_coord = {0.0f, 1.0f}
                    };
                    Vertex v3 = {
                        .position = {x - 1.0f, y + 1.0f, z + 1.0f},
                        .normal = {-1.0f, 0.0f, 0.0f},
                        .texture_coord = {1.0f, 1.0f}
                    };
                    
                    chunk.vertices[chunk.number_of_vertices++] = v0;
                    chunk.vertices[chunk.number_of_vertices++] = v1;
                    chunk.vertices[chunk.number_of_vertices++] = v2;
                    chunk.vertices[chunk.number_of_vertices++] = v3;

                    unsigned int base_index = chunk.number_of_vertices - 4;
                    chunk.indices[chunk.number_of_indices++] = base_index;
                    chunk.indices[chunk.number_of_indices++] = base_index + 1;
                    chunk.indices[chunk.number_of_indices++] = base_index + 2;

                    chunk.indices[chunk.number_of_indices++] = base_index;
                    chunk.indices[chunk.number_of_indices++] = base_index + 2;
                    chunk.indices[chunk.number_of_indices++] = base_index + 3;
                }

            }
        }
    }

    glGenVertexArrays(1, &chunk.VAO);
    glBindVertexArray(chunk.VAO);

    glGenBuffers(1, &chunk.VBO);
    glBindBuffer(GL_ARRAY_BUFFER, chunk.VBO);

    // buffer chunk vertices TEMP
    glBufferData(GL_ARRAY_BUFFER, chunk.number_of_vertices * sizeof(Vertex), chunk.vertices, GL_STATIC_DRAW);

    glGenBuffers(1, &chunk.EBO);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, chunk.EBO);

    // buffer chunk indices TEMP
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, chunk.number_of_indices * sizeof(unsigned int), chunk.indices, GL_STATIC_DRAW);

    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*) offsetof(Vertex, position));
    glEnableVertexAttribArray(0);

    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*) offsetof(Vertex, normal));
    glEnableVertexAttribArray(1);

    glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*) offsetof(Vertex, texture_coord));
    glEnableVertexAttribArray(2);

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindVertexArray(0);

    chunk.model = matrix4f_identity();

    return chunk;
}

void chunk_draw(Chunk* chunk) {
    glBindVertexArray(chunk->VAO);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, chunk->EBO);
    glDrawElements(GL_TRIANGLES, chunk->number_of_indices, GL_UNSIGNED_INT, 0);
}