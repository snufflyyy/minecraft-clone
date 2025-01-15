#include "chunk.h"

#include <stdlib.h>
#include <stddef.h>
#include <stdio.h>

#include <glad/glad.h>

Chunk chunk_create() {
    Chunk chunk = {0};

    // temp until world generation is done
    // just setting all the blocks to grass for now
    for (int x = 0; x < CHUNK_SIZE; x++) {
        for (int z = 0; z < CHUNK_SIZE; z++) {
            int random_y = rand() % CHUNK_SIZE;
            for (int y = 0; y < CHUNK_SIZE; y++) {
                if (y <= random_y) {
                    chunk.blocks[x][y][z].type = GRASS;
                }
            }
        }
    }

    chunk_generate_mesh(&chunk);

    glGenVertexArrays(1, &chunk.VAO);
    glBindVertexArray(chunk.VAO);

    glGenBuffers(1, &chunk.VBO);
    glBindBuffer(GL_ARRAY_BUFFER, chunk.VBO);

    glBufferData(GL_ARRAY_BUFFER, chunk.number_of_vertices * sizeof(Vertex), chunk.vertices, GL_STATIC_DRAW);

    glGenBuffers(1, &chunk.EBO);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, chunk.EBO);

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

void chunk_generate_mesh(Chunk* chunk) {
    chunk->number_of_vertices = 0;
    chunk->number_of_indices = 0;

    int max_vertices = CHUNK_SIZE * CHUNK_SIZE * CHUNK_SIZE * 4 * 6;
    int max_indices = CHUNK_SIZE * CHUNK_SIZE * CHUNK_SIZE * 6 * 6;

    chunk->vertices = malloc(max_vertices * sizeof(Vertex));
    chunk->indices = malloc(max_indices * sizeof(unsigned int));

    for (int x = 0; x < CHUNK_SIZE; x++) {
        for (int y = 0; y < CHUNK_SIZE; y++) {
            for (int z = 0; z < CHUNK_SIZE; z++) {
                if (chunk->blocks[x][y][z].type == AIR) { continue; }

                // generate top face
                if (y == CHUNK_SIZE - 1 || chunk->blocks[x][y + 1][z].type == AIR) {
                    Vertex v0 = {
                        .position = {x - 0.5f, y + 0.5f, z + 0.5f},
                        .normal = {0.0f, 1.0f, 0.0f},
                        .texture_coord = {1.0f, 0.0f}
                    };
                    Vertex v1 = {
                        .position = {x + 0.5f, y + 0.5f, z + 0.5f},
                        .normal = {0.0f, 1.0f, 0.0f},
                        .texture_coord = {0.0f, 0.0f}
                    };
                    Vertex v2 = {
                        .position = {x + 0.5f, y + 0.5f, z - 0.5f},
                        .normal = {0.0f, 1.0f, 0.0f},
                        .texture_coord = {0.0f, 1.0f}
                    };
                    Vertex v3 = {
                        .position = {x - 0.5f, y + 0.5f, z - 0.5f},
                        .normal = {0.0f, 1.0f, 0.0f},
                        .texture_coord = {1.0f, 1.0f}
                    };

                    chunk->vertices[chunk->number_of_vertices++] = v0;
                    chunk->vertices[chunk->number_of_vertices++] = v1;
                    chunk->vertices[chunk->number_of_vertices++] = v2;
                    chunk->vertices[chunk->number_of_vertices++] = v3;

                    unsigned int base_index = chunk->number_of_vertices - 4;
                    chunk->indices[chunk->number_of_indices++] = base_index;
                    chunk->indices[chunk->number_of_indices++] = base_index + 1;
                    chunk->indices[chunk->number_of_indices++] = base_index + 2;

                    chunk->indices[chunk->number_of_indices++] = base_index;
                    chunk->indices[chunk->number_of_indices++] = base_index + 2;
                    chunk->indices[chunk->number_of_indices++] = base_index + 3;
                }

                // generate bottom face
                if (y == 0 || chunk->blocks[x][y - 1][z].type == AIR) {
                    Vertex v0 = {
                        .position = {x - 0.5f, y - 0.5f, z + 0.5f},
                        .normal = {0.0f, -1.0f, 0.0f},
                        .texture_coord = {1.0f, 0.0f}
                    };
                    Vertex v1 = {
                        .position = {x + 0.5f, y - 0.5f, z + 0.5f},
                        .normal = {0.0f, -1.0f, 0.0f},
                        .texture_coord = {0.0f, 0.0f}
                    };
                    Vertex v2 = {
                        .position = {x + 0.5f, y - 0.5f, z - 0.5f},
                        .normal = {0.0f, -1.0f, 0.0f},
                        .texture_coord = {0.0f, 1.0f}
                    };
                    Vertex v3 = {
                        .position = {x - 0.5f, y - 0.5f, z - 0.5f},
                        .normal = {0.0f, -1.0f, 0.0f},
                        .texture_coord = {1.0f, 1.0f}
                    };

                    chunk->vertices[chunk->number_of_vertices++] = v0;
                    chunk->vertices[chunk->number_of_vertices++] = v1;
                    chunk->vertices[chunk->number_of_vertices++] = v2;
                    chunk->vertices[chunk->number_of_vertices++] = v3;

                    unsigned int base_index = chunk->number_of_vertices - 4;
                    chunk->indices[chunk->number_of_indices++] = base_index;
                    chunk->indices[chunk->number_of_indices++] = base_index + 2;
                    chunk->indices[chunk->number_of_indices++] = base_index + 1;

                    chunk->indices[chunk->number_of_indices++] = base_index;
                    chunk->indices[chunk->number_of_indices++] = base_index + 3;
                    chunk->indices[chunk->number_of_indices++] = base_index + 2;
                }

                // generate forward face
                if (z == 0 || chunk->blocks[x][y][z - 1].type == AIR) {
                    Vertex v0 = {
                        .position = {x - 0.5f, y - 0.5f, z - 0.5f},
                        .normal = {0.0f, 0.0f, -1.0f},
                        .texture_coord = {1.0f, 0.0f}
                    };
                    Vertex v1 = {
                        .position = {x + 0.5f, y - 0.5f, z - 0.5f},
                        .normal = {0.0f, 0.0f, -1.0f},
                        .texture_coord = {0.0f, 0.0f}
                    };
                    Vertex v2 = {
                        .position = {x + 0.5f, y + 0.5f, z - 0.5f},
                        .normal = {0.0f, 0.0f, -1.0f},
                        .texture_coord = {0.0f, 1.0f}
                    };
                    Vertex v3 = {
                        .position = {x - 0.5f, y + 0.5f, z - 0.5f},
                        .normal = {0.0f, 0.0f, -1.0f},
                        .texture_coord = {1.0f, 1.0f}
                    };

                    chunk->vertices[chunk->number_of_vertices++] = v0;
                    chunk->vertices[chunk->number_of_vertices++] = v1;
                    chunk->vertices[chunk->number_of_vertices++] = v2;
                    chunk->vertices[chunk->number_of_vertices++] = v3;

                    unsigned int base_index = chunk->number_of_vertices - 4;
                    chunk->indices[chunk->number_of_indices++] = base_index;
                    chunk->indices[chunk->number_of_indices++] = base_index + 2;
                    chunk->indices[chunk->number_of_indices++] = base_index + 1;

                    chunk->indices[chunk->number_of_indices++] = base_index;
                    chunk->indices[chunk->number_of_indices++] = base_index + 3;
                    chunk->indices[chunk->number_of_indices++] = base_index + 2;
                }

                // generate backward face
                if (z == CHUNK_SIZE - 1 || chunk->blocks[x][y][z + 1].type == AIR) {
                    Vertex v0 = {
                        .position = {x - 0.5f, y - 0.5f, z + 0.5f},
                        .normal = {0.0f, 0.0f, 1.0f},
                        .texture_coord = {1.0f, 0.0f}
                    };
                    Vertex v1 = {
                        .position = {x + 0.5f, y - 0.5f, z + 0.5f},
                        .normal = {0.0f, 0.0f, 1.0f},
                        .texture_coord = {0.0f, 0.0f}
                    };
                    Vertex v2 = {
                        .position = {x + 0.5f, y + 0.5f, z + 0.5f},
                        .normal = {0.0f, 0.0f, 1.0f},
                        .texture_coord = {0.0f, 1.0f}
                    };
                    Vertex v3 = {
                        .position = {x - 0.5f, y + 0.5f, z + 0.5f},
                        .normal = {0.0f, 0.0f, 1.0f},
                        .texture_coord = {1.0f, 1.0f}
                    };

                    chunk->vertices[chunk->number_of_vertices++] = v0;
                    chunk->vertices[chunk->number_of_vertices++] = v1;
                    chunk->vertices[chunk->number_of_vertices++] = v2;
                    chunk->vertices[chunk->number_of_vertices++] = v3;

                    unsigned int base_index = chunk->number_of_vertices - 4;
                    chunk->indices[chunk->number_of_indices++] = base_index;
                    chunk->indices[chunk->number_of_indices++] = base_index + 1;
                    chunk->indices[chunk->number_of_indices++] = base_index + 2;

                    chunk->indices[chunk->number_of_indices++] = base_index;
                    chunk->indices[chunk->number_of_indices++] = base_index + 2;
                    chunk->indices[chunk->number_of_indices++] = base_index + 3;
                }

                // generate left face
                if (x == 0 || chunk->blocks[x - 1][y][z].type == AIR) {
                    Vertex v0 = {
                        .position = {x - 0.5f, y - 0.5f, z + 0.5f},
                        .normal = {-1.0f, 0.0f, 0.0f},
                        .texture_coord = {1.0f, 0.0f}
                    };
                    Vertex v1 = {
                        .position = {x - 0.5f, y - 0.5f, z - 0.5f},
                        .normal = {-1.0f, 0.0f, 0.0f},
                        .texture_coord = {0.0f, 0.0f}
                    };
                    Vertex v2 = {
                        .position = {x - 0.5f, y + 0.5f, z - 0.5f},
                        .normal = {-1.0f, 0.0f, 0.0f},
                        .texture_coord = {0.0f, 1.0f}
                    };
                    Vertex v3 = {
                        .position = {x - 0.5f, y + 0.5f, z + 0.5f},
                        .normal = {-1.0f, 0.0f, 0.0f},
                        .texture_coord = {1.0f, 1.0f}
                    };

                    chunk->vertices[chunk->number_of_vertices++] = v0;
                    chunk->vertices[chunk->number_of_vertices++] = v1;
                    chunk->vertices[chunk->number_of_vertices++] = v2;
                    chunk->vertices[chunk->number_of_vertices++] = v3;

                    unsigned int base_index = chunk->number_of_vertices - 4;
                    chunk->indices[chunk->number_of_indices++] = base_index;
                    chunk->indices[chunk->number_of_indices++] = base_index + 2;
                    chunk->indices[chunk->number_of_indices++] = base_index + 1;

                    chunk->indices[chunk->number_of_indices++] = base_index;
                    chunk->indices[chunk->number_of_indices++] = base_index + 3;
                    chunk->indices[chunk->number_of_indices++] = base_index + 2;
                }

                // generate right face
                if (x == CHUNK_SIZE - 1 || chunk->blocks[x + 1][y][z].type == AIR) {
                    Vertex v0 = {
                        .position = {x + 0.5f, y - 0.5f, z + 0.5f},
                        .normal = {1.0f, 0.0f, 0.0f},
                        .texture_coord = {1.0f, 0.0f}
                    };
                    Vertex v1 = {
                        .position = {x + 0.5f, y - 0.5f, z - 0.5f},
                        .normal = {1.0f, 0.0f, 0.0f},
                        .texture_coord = {0.0f, 0.0f}
                    };
                    Vertex v2 = {
                        .position = {x + 0.5f, y + 0.5f, z - 0.5f},
                        .normal = {1.0f, 0.0f, 0.0f},
                        .texture_coord = {0.0f, 1.0f}
                    };
                    Vertex v3 = {
                        .position = {x + 0.5f, y + 0.5f, z + 0.5f},
                        .normal = {1.0f, 0.0f, 0.0f},
                        .texture_coord = {1.0f, 1.0f}
                    };

                    chunk->vertices[chunk->number_of_vertices++] = v0;
                    chunk->vertices[chunk->number_of_vertices++] = v1;
                    chunk->vertices[chunk->number_of_vertices++] = v2;
                    chunk->vertices[chunk->number_of_vertices++] = v3;

                    unsigned int base_index = chunk->number_of_vertices - 4;
                    chunk->indices[chunk->number_of_indices++] = base_index;
                    chunk->indices[chunk->number_of_indices++] = base_index + 1;
                    chunk->indices[chunk->number_of_indices++] = base_index + 2;

                    chunk->indices[chunk->number_of_indices++] = base_index;
                    chunk->indices[chunk->number_of_indices++] = base_index + 2;
                    chunk->indices[chunk->number_of_indices++] = base_index + 3;
                }
            }
        }
    }
}

void chunk_draw(Chunk* chunk) {
    glBindVertexArray(chunk->VAO);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, chunk->EBO);
    glDrawElements(GL_TRIANGLES, chunk->number_of_indices, GL_UNSIGNED_INT, 0);
}

void chunk_delete(Chunk* chunk) {
    free(chunk->indices);
    free(chunk->vertices);

    glDeleteBuffers(1, &chunk->VBO);
    glDeleteBuffers(1, &chunk->EBO);
    glDeleteVertexArrays(1, &chunk->VAO);
}