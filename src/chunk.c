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
        for (int y = 0; y < CHUNK_SIZE; y++) {
            for (int z = 0; z < CHUNK_SIZE; z++) {
                chunk.blocks[x * (CHUNK_SIZE * CHUNK_SIZE) + y * CHUNK_SIZE + z].type = GRASS;
            }
        }
    }

    glGenVertexArrays(1, &chunk.VAO);
    glGenBuffers(1, &chunk.VBO);
    glGenBuffers(1, &chunk.EBO);

    return chunk;
}

void chunk_update(Chunk* chunk, Chunk* neighbors[6]) {
    chunk->number_of_vertices = 0;
    chunk->number_of_indices = 0;

    for (int x = 0; x < CHUNK_SIZE; x++) {
        for (int y = 0; y < CHUNK_SIZE; y++) {
            for (int z = 0; z < CHUNK_SIZE; z++) {
                // in current chunk
                int current_index = x * (CHUNK_SIZE * CHUNK_SIZE) + y * CHUNK_SIZE + z;
                int above_index   = x * (CHUNK_SIZE * CHUNK_SIZE) + (y + 1) * CHUNK_SIZE + z;
                int below_index   = x * (CHUNK_SIZE * CHUNK_SIZE) + (y - 1) * CHUNK_SIZE + z;
                int left_index    = (x - 1) * (CHUNK_SIZE * CHUNK_SIZE) + y * CHUNK_SIZE + z;
                int right_index   = (x + 1) * (CHUNK_SIZE * CHUNK_SIZE) + y * CHUNK_SIZE + z;
                int infront_index = x * (CHUNK_SIZE * CHUNK_SIZE) + y * CHUNK_SIZE + (z + 1);
                int behind_index  = x * (CHUNK_SIZE * CHUNK_SIZE) + y * CHUNK_SIZE + (z - 1);

                // neighboring chunks
                int top_neighbour      = x * (CHUNK_SIZE * CHUNK_SIZE) + (CHUNK_SIZE - 1) * CHUNK_SIZE + z;
                int bottom_neighbour   = x * (CHUNK_SIZE * CHUNK_SIZE) + 0 * CHUNK_SIZE + z;
                int left_neighbour     = 0 * (CHUNK_SIZE * CHUNK_SIZE) + y * CHUNK_SIZE + z;
                int right_neighbour    = (CHUNK_SIZE - 1) * (CHUNK_SIZE * CHUNK_SIZE) + y * CHUNK_SIZE + z;
                int forward_neighbour  = x * (CHUNK_SIZE * CHUNK_SIZE) + y * CHUNK_SIZE + (CHUNK_SIZE - 1);
                int backward_neighbour = x * (CHUNK_SIZE * CHUNK_SIZE) + y * CHUNK_SIZE + 0;

                // if current block is air just skip it
                if (chunk->blocks[current_index].type == AIR) { continue; }

                bool visible = false;

                // top face
                if (y == CHUNK_SIZE - 1) {
                    visible = neighbors[0] ? neighbors[0]->blocks[bottom_neighbour].type == AIR : true;
                } else {
                    visible = chunk->blocks[above_index].type == AIR;
                }
                chunk->number_of_vertices += visible ? 4 : 0;
                chunk->number_of_indices += visible ? 6 : 0;
                chunk->blocks[current_index].is_top_face_visible = visible;
                visible = false;

                // bottom face
                if (y == 0) {
                    visible = neighbors[1] ? neighbors[1]->blocks[top_neighbour].type == AIR : true;
                } else {
                    visible = chunk->blocks[below_index].type == AIR;
                }
                chunk->number_of_vertices += visible ? 4 : 0;
                chunk->number_of_indices += visible ? 6 : 0;
                chunk->blocks[current_index].is_bottom_face_visible = visible;
                visible = false;

                // left face
                if (x == 0) {
                    visible = neighbors[2] ? neighbors[2]->blocks[right_neighbour].type == AIR : true;
                } else {
                    visible = chunk->blocks[left_index].type == AIR;
                }
                chunk->number_of_vertices += visible ? 4 : 0;
                chunk->number_of_indices += visible ? 6 : 0;
                chunk->blocks[current_index].is_left_face_visible = visible;
                visible = false;

                // right face
                if (x == CHUNK_SIZE - 1) {
                    visible = neighbors[3] ? neighbors[3]->blocks[left_neighbour].type == AIR : true;
                } else {
                    visible = chunk->blocks[right_index].type == AIR;
                }
                chunk->number_of_vertices += visible ? 4 : 0;
                chunk->number_of_indices += visible ? 6 : 0;
                chunk->blocks[current_index].is_right_face_visible = visible;
                visible = false;

                // forward face
                if (z == CHUNK_SIZE - 1) {
                    visible = neighbors[4] ? neighbors[4]->blocks[backward_neighbour].type == AIR : true;
                } else {
                    visible = chunk->blocks[infront_index].type == AIR;
                }
                chunk->number_of_vertices += visible ? 4 : 0;
                chunk->number_of_indices += visible ? 6 : 0;
                chunk->blocks[current_index].is_forward_face_visible = visible;
                visible = false;

                // backward face
                if (z == 0) {
                    visible = neighbors[5] ? neighbors[5]->blocks[forward_neighbour].type == AIR : true;
                } else {
                    visible = chunk->blocks[backward_neighbour].type == AIR;
                }
                chunk->number_of_vertices += visible ? 4 : 0;
                chunk->number_of_indices += visible ? 6 : 0;
                chunk->blocks[current_index].is_backward_face_visible = visible;
                visible = false;
            }
        }
    }
}

void chunk_generate_mesh(Chunk* chunk) {
    chunk->vertices = malloc(chunk->number_of_vertices * sizeof(Vertex));
    if (!chunk->vertices) {
        printf("ERROR: Failed to allocate memory for chunk vertices!");
        exit(-6);
    }
    chunk->indices = malloc(chunk->number_of_indices * sizeof(unsigned int));
    if (!chunk->indices) {
        printf("ERROR: Failed to allocate memory for chunk indices!");
        exit(-7);
    }

    chunk->number_of_vertices = 0;
    chunk->number_of_indices = 0;

    for (int x = 0; x < CHUNK_SIZE; x++) {
        for (int y = 0; y < CHUNK_SIZE; y++) {
            for (int z = 0; z < CHUNK_SIZE; z++) {
                int current_index = x * (CHUNK_SIZE * CHUNK_SIZE) + y * CHUNK_SIZE + z;

                // if current block is air just skip it
                if (chunk->blocks[current_index].type == AIR) { continue; }

                // generate top face
                if (chunk->blocks[current_index].is_top_face_visible) {
                    chunk->vertices[chunk->number_of_vertices++] = (Vertex) {
                        .position = {x - 0.5f, y + 0.5f, z + 0.5f},
                        .normal = {0.0f, 1.0f, 0.0f},
                        .texture_coord = {1.0f, 0.0f}
                    };
                    chunk->vertices[chunk->number_of_vertices++] = (Vertex) {
                        .position = {x + 0.5f, y + 0.5f, z + 0.5f},
                        .normal = {0.0f, 1.0f, 0.0f},
                        .texture_coord = {0.0f, 0.0f}
                    };
                    chunk->vertices[chunk->number_of_vertices++] = (Vertex) {
                        .position = {x + 0.5f, y + 0.5f, z - 0.5f},
                        .normal = {0.0f, 1.0f, 0.0f},
                        .texture_coord = {0.0f, 1.0f}
                    };        
                    chunk->vertices[chunk->number_of_vertices++] = (Vertex) {
                        .position = {x - 0.5f, y + 0.5f, z - 0.5f},
                        .normal = {0.0f, 1.0f, 0.0f},
                        .texture_coord = {1.0f, 1.0f}
                    }; 

                    unsigned int base_index = chunk->number_of_vertices - 4;
                    // first triangle
                    chunk->indices[chunk->number_of_indices++] = base_index;
                    chunk->indices[chunk->number_of_indices++] = base_index + 1;
                    chunk->indices[chunk->number_of_indices++] = base_index + 2;

                    // second triangle
                    chunk->indices[chunk->number_of_indices++] = base_index;
                    chunk->indices[chunk->number_of_indices++] = base_index + 2;
                    chunk->indices[chunk->number_of_indices++] = base_index + 3;
                }

                // generate bottom face
                if (chunk->blocks[current_index].is_bottom_face_visible) {
                    chunk->vertices[chunk->number_of_vertices++] = (Vertex) {
                        .position = {x - 0.5f, y - 0.5f, z + 0.5f},
                        .normal = {0.0f, -1.0f, 0.0f},
                        .texture_coord = {1.0f, 0.0f}
                    };
                    chunk->vertices[chunk->number_of_vertices++] = (Vertex) {
                        .position = {x + 0.5f, y - 0.5f, z + 0.5f},
                        .normal = {0.0f, -1.0f, 0.0f},
                        .texture_coord = {0.0f, 0.0f}
                    };
                    chunk->vertices[chunk->number_of_vertices++] = (Vertex) {
                        .position = {x + 0.5f, y - 0.5f, z - 0.5f},
                        .normal = {0.0f, -1.0f, 0.0f},
                        .texture_coord = {0.0f, 1.0f}
                    };
                    chunk->vertices[chunk->number_of_vertices++] = (Vertex) {
                        .position = {x - 0.5f, y - 0.5f, z - 0.5f},
                        .normal = {0.0f, -1.0f, 0.0f},
                        .texture_coord = {1.0f, 1.0f}
                    };

                    unsigned int base_index = chunk->number_of_vertices - 4;
                    // first triangle
                    chunk->indices[chunk->number_of_indices++] = base_index;
                    chunk->indices[chunk->number_of_indices++] = base_index + 2;
                    chunk->indices[chunk->number_of_indices++] = base_index + 1;
                    
                    // second triangle
                    chunk->indices[chunk->number_of_indices++] = base_index;
                    chunk->indices[chunk->number_of_indices++] = base_index + 3;
                    chunk->indices[chunk->number_of_indices++] = base_index + 2;
                }

                // generate left face
                if (chunk->blocks[current_index].is_left_face_visible) {
                    chunk->vertices[chunk->number_of_vertices++] = (Vertex) {
                        .position = {x - 0.5f, y - 0.5f, z + 0.5f},
                        .normal = {-1.0f, 0.0f, 0.0f},
                        .texture_coord = {1.0f, 0.0f}
                    };
                    chunk->vertices[chunk->number_of_vertices++] = (Vertex) {
                        .position = {x - 0.5f, y - 0.5f, z - 0.5f},
                        .normal = {-1.0f, 0.0f, 0.0f},
                        .texture_coord = {0.0f, 0.0f}
                    };
                    chunk->vertices[chunk->number_of_vertices++] = (Vertex) {
                        .position = {x - 0.5f, y + 0.5f, z - 0.5f},
                        .normal = {-1.0f, 0.0f, 0.0f},
                        .texture_coord = {0.0f, 1.0f}
                    };
                    chunk->vertices[chunk->number_of_vertices++] = (Vertex) {
                        .position = {x - 0.5f, y + 0.5f, z + 0.5f},
                        .normal = {-1.0f, 0.0f, 0.0f},
                        .texture_coord = {1.0f, 1.0f}
                    };

                    unsigned int base_index = chunk->number_of_vertices - 4;
                    // first triangle
                    chunk->indices[chunk->number_of_indices++] = base_index;
                    chunk->indices[chunk->number_of_indices++] = base_index + 2;
                    chunk->indices[chunk->number_of_indices++] = base_index + 1;

                    // second triangle
                    chunk->indices[chunk->number_of_indices++] = base_index;
                    chunk->indices[chunk->number_of_indices++] = base_index + 3;
                    chunk->indices[chunk->number_of_indices++] = base_index + 2;
                }

                // generate right face
                if (chunk->blocks[current_index].is_right_face_visible) {
                    chunk->vertices[chunk->number_of_vertices++] = (Vertex) {
                        .position = {x + 0.5f, y - 0.5f, z + 0.5f},
                        .normal = {1.0f, 0.0f, 0.0f},
                        .texture_coord = {1.0f, 0.0f}
                    };
                    chunk->vertices[chunk->number_of_vertices++] = (Vertex) {
                        .position = {x + 0.5f, y - 0.5f, z - 0.5f},
                        .normal = {1.0f, 0.0f, 0.0f},
                        .texture_coord = {0.0f, 0.0f}
                    };
                    chunk->vertices[chunk->number_of_vertices++] = (Vertex) {
                        .position = {x + 0.5f, y + 0.5f, z - 0.5f},
                        .normal = {1.0f, 0.0f, 0.0f},
                        .texture_coord = {0.0f, 1.0f}
                    };
                    chunk->vertices[chunk->number_of_vertices++] = (Vertex) {
                        .position = {x + 0.5f, y + 0.5f, z + 0.5f},
                        .normal = {1.0f, 0.0f, 0.0f},
                        .texture_coord = {1.0f, 1.0f}
                    };

                    unsigned int base_index = chunk->number_of_vertices - 4;
                    // first triangle
                    chunk->indices[chunk->number_of_indices++] = base_index;
                    chunk->indices[chunk->number_of_indices++] = base_index + 1;
                    chunk->indices[chunk->number_of_indices++] = base_index + 2;

                    // second triangle
                    chunk->indices[chunk->number_of_indices++] = base_index;
                    chunk->indices[chunk->number_of_indices++] = base_index + 2;
                    chunk->indices[chunk->number_of_indices++] = base_index + 3;
                }

                // generate forward face
                if (chunk->blocks[current_index].is_forward_face_visible) {
                    chunk->vertices[chunk->number_of_vertices++] = (Vertex) {
                        .position = {x - 0.5f, y - 0.5f, z + 0.5f},
                        .normal = {0.0f, 0.0f, -1.0f},
                        .texture_coord = {1.0f, 0.0f}
                    };
                    chunk->vertices[chunk->number_of_vertices++] = (Vertex) {
                        .position = {x + 0.5f, y - 0.5f, z + 0.5f},
                        .normal = {0.0f, 0.0f, -1.0f},
                        .texture_coord = {0.0f, 0.0f}
                    };
                    chunk->vertices[chunk->number_of_vertices++] = (Vertex) {
                        .position = {x + 0.5f, y + 0.5f, z + 0.5f},
                        .normal = {0.0f, 0.0f, -1.0f},
                        .texture_coord = {0.0f, 1.0f}
                    };
                    chunk->vertices[chunk->number_of_vertices++] = (Vertex) {
                        .position = {x - 0.5f, y + 0.5f, z + 0.5f},
                        .normal = {0.0f, 0.0f, -1.0f},
                        .texture_coord = {1.0f, 1.0f}
                    };

                    unsigned int base_index = chunk->number_of_vertices - 4;
                    // first triangle
                    chunk->indices[chunk->number_of_indices++] = base_index;
                    chunk->indices[chunk->number_of_indices++] = base_index + 1;
                    chunk->indices[chunk->number_of_indices++] = base_index + 2;

                    // second triangle
                    chunk->indices[chunk->number_of_indices++] = base_index;
                    chunk->indices[chunk->number_of_indices++] = base_index + 2;
                    chunk->indices[chunk->number_of_indices++] = base_index + 3;
                }

                // generate backward face
                if (chunk->blocks[current_index].is_backward_face_visible) {
                    chunk->vertices[chunk->number_of_vertices++] = (Vertex) {
                        .position = {x - 0.5f, y - 0.5f, z - 0.5f},
                        .normal = {0.0f, 0.0f, 1.0f},
                        .texture_coord = {1.0f, 0.0f}
                    };
                    chunk->vertices[chunk->number_of_vertices++] = (Vertex) {
                        .position = {x + 0.5f, y - 0.5f, z - 0.5f},
                        .normal = {0.0f, 0.0f, 1.0f},
                        .texture_coord = {0.0f, 0.0f}
                    };
                    chunk->vertices[chunk->number_of_vertices++] = (Vertex) {
                        .position = {x + 0.5f, y + 0.5f, z - 0.5f},
                        .normal = {0.0f, 0.0f, 1.0f},
                        .texture_coord = {0.0f, 1.0f}
                    };
                    chunk->vertices[chunk->number_of_vertices++] = (Vertex) {
                        .position = {x - 0.5f, y + 0.5f, z - 0.5f},
                        .normal = {0.0f, 0.0f, 1.0f},
                        .texture_coord = {1.0f, 1.0f}
                    };

                    unsigned int base_index = chunk->number_of_vertices - 4;
                    // first triangle
                    chunk->indices[chunk->number_of_indices++] = base_index;
                    chunk->indices[chunk->number_of_indices++] = base_index + 2;
                    chunk->indices[chunk->number_of_indices++] = base_index + 1;

                    // second triangle
                    chunk->indices[chunk->number_of_indices++] = base_index;
                    chunk->indices[chunk->number_of_indices++] = base_index + 3;
                    chunk->indices[chunk->number_of_indices++] = base_index + 2;
                }
            }
        }
    }

    glBindVertexArray(chunk->VAO);

    glBindBuffer(GL_ARRAY_BUFFER, chunk->VBO);
    glBufferData(GL_ARRAY_BUFFER, chunk->number_of_vertices * sizeof(Vertex), chunk->vertices, GL_STATIC_DRAW);

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, chunk->EBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, chunk->number_of_indices * sizeof(unsigned int), chunk->indices, GL_STATIC_DRAW);

    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*) offsetof(Vertex, position));
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*) offsetof(Vertex, normal));
    glEnableVertexAttribArray(1);
    glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*) offsetof(Vertex, texture_coord));
    glEnableVertexAttribArray(2);

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindVertexArray(0);
}

void chunk_draw(Chunk* chunk) {
    glBindVertexArray(chunk->VAO);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, chunk->EBO);
    glDrawElements(GL_TRIANGLES, chunk->number_of_indices, GL_UNSIGNED_INT, 0);
}

void chunk_destroy(Chunk* chunk) {
    free(chunk->indices);
    chunk->indices = NULL;
    free(chunk->vertices);
    chunk->vertices = NULL;
    
    glDeleteBuffers(1, &chunk->EBO);
    glDeleteBuffers(1, &chunk->VBO);
    glDeleteVertexArrays(1, &chunk->VAO);
}