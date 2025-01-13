#include "chunk.h"

#include <glad/glad.h>

Chunk chunk_create() {
    Chunk chunk = {0};

    // temp until world generation is done
    // just setting all the blocks to grass for now
    for (int x = 0; x < CHUNK_SIZE; x++) {
        for (int y = 0; y < CHUNK_SIZE; y++) {
            for (int z = 0; z < CHUNK_SIZE; z++) {
                chunk.blocks[x][y][z].type = GRASS;

                chunk.blocks[x][y][z].is_top_face_visable = true;
                chunk.blocks[x][y][z].is_bottom_face_visable = true;
                chunk.blocks[x][y][z].is_left_face_visable = true;
                chunk.blocks[x][y][z].is_right_face_visable = true;
                chunk.blocks[x][y][z].is_forward_face_visable = true;
                chunk.blocks[x][y][z].is_backward_face_visable = true;
            }
        }
    }

    for (int x = 0; x < CHUNK_SIZE; x++) {
        for (int y = 0; y < CHUNK_SIZE; y++) {
            for (int z = 0; z < CHUNK_SIZE; z++) {
                if (chunk.blocks[x][y][z].type == AIR) { continue; }

                chunk.blocks[x][y][z].is_left_face_visable     = (x == 0 || chunk.blocks[x - 1][y][z].type == AIR);
                chunk.blocks[x][y][z].is_right_face_visable    = (x == CHUNK_SIZE - 1 || chunk.blocks[x + 1][y][z].type == AIR);
                chunk.blocks[x][y][z].is_top_face_visable      = (y == 0 || chunk.blocks[x][y - 1][z].type == AIR);
                chunk.blocks[x][y][z].is_bottom_face_visable   = (y == CHUNK_SIZE - 1 || chunk.blocks[x + 1][y][z].type == AIR);
                chunk.blocks[x][y][z].is_forward_face_visable  = (z == 0 || chunk.blocks[x][y][z + 1].type == AIR);
                chunk.blocks[x][y][z].is_backward_face_visable = (z == CHUNK_SIZE - 1 || chunk.blocks[x][y][z - 1].type == AIR);
            }
        }
    }

    glGenVertexArrays(1, &chunk.VAO);
    glBindVertexArray(chunk.VAO);

    glGenBuffers(1, &chunk.VBO);
    glBindBuffer(GL_ARRAY_BUFFER, chunk.VBO);

    // buffer chunk vertices

    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindVertexArray(0);

    return chunk;
}
